#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>

int slaves;
int myid;
long strips;
int nprocs;
long long runtime;

long long wall_clock_time()
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME,&tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec*1000000000ll);
}

long getUpperBound(double x)
{
        float y;
        y = 4.0/(1+pow(x,2))*strips;
        //printf ("minus: %f \n", y-(long)y);
        if (y-(long)y > 0)
                y = (long)y+1;
        return (long)y;
}

long getLowerBound(double x)
{
        float y;
        y = 4.0/(1+pow(x,2))*strips;
        return (long)y;
}

void slave_computation(double start, long iterations)
{	
	int i;
	double size = 1.0/strips;
	long localSum[2]={0,0};
	double xleft, xright;
	long sum[2]={0,0};
	for (i=0; i<iterations; i++)
	{
		xleft=start+i*size;
		xright=start+(i+1)*size;
		localSum[0]+=getUpperBound(xleft);
		localSum[1]+=getLowerBound(xright);
	}
	MPI_Reduce(&localSum,&sum,2,MPI_LONG,MPI_SUM,nprocs-1,MPI_COMM_WORLD);
	//printf("id: %d, sum: %li \n", myid,sum[0]);
	if(myid == nprocs-1)
	{
		double result = (sum[0]+sum[1])/2.0*(size*size);
		printf("Integration result: %.8f", result);
	}
}

void slave()
{
	long strips_per_slave = strips/slaves;
	int extra_strips = strips % slaves;
	double size = 1.0/strips;
	double start=0;
	long long before, after;
	before = wall_clock_time();
	if (myid < extra_strips)
	{	
		//do 1 extra iteration
		start = myid*size*(strips_per_slave+1);
		slave_computation(start, strips_per_slave+1);
	}
	else 
	{
		start = myid*size*strips_per_slave + extra_strips*size;
		slave_computation(start, strips_per_slave);
	}
	after = wall_clock_time();
	runtime += after - before;
	if (myid == nprocs -1)
	{
		printf(",took %.3f seconds using %li strips and %d threads \n",runtime/1000000000.0, strips, nprocs);
	}
}

int main(int argc, char **argv)
{
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	if(argc>=2)
	{
		strips = atol(argv[1]);
	}
	else
	{	
		strips = 10;
	}	
	slaves = nprocs;
	slave();
	MPI_Finalize();
	return 0;
}

