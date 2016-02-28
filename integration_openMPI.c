#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
int slaves;
int myid;
long strips = 1000;
int nprocs;

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
	long upper=0;
	long lower=0;
	double xleft, xright;
	long sum[2];
	printf("size: %f, start: %f, id: %d \n", size, start, myid);
	for (i=0; i<iterations; i++)
	{
		xleft=start+i*size;
		xright=start+(i+1)*size;
		upper+=getUpperBound(xleft);
		lower+=getLowerBound(xright);
	}
	MPI_Scan(&upper,&sum[0],1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
	MPI_Scan(&lower,&sum[1],1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
	//printf("upper: %li, lower: %li, id: %d \n", upper, lower, myid);
	if(myid == nprocs-1)
	{
		printf("Sumupper: %li, Sumlower: %li \n", sum[0], sum[1]);
	}
}

void slave()
{
	long strips_per_slave = strips/slaves;
	printf("%li \n", strips_per_slave);
	int extra_strips = strips % slaves;
	printf("%d \n", extra_strips);
	double size = 1.0/strips;
	double start=0;
	if (myid < extra_strips)
	{	
		//do 1 extra iteration
		printf("strips_per_slave: %li, \n", strips_per_slave);
		start = myid*size*(strips_per_slave+1);
		slave_computation(start, strips_per_slave+1);
	}
	else 
	{
		start = myid*size*strips_per_slave + extra_strips*size;
		slave_computation(start, strips_per_slave);
	}
}

int main(int argc, char **argv)
{
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	slaves = nprocs;
	slave();
	MPI_Finalize();
	return 0;
}

