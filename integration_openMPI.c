#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int slaves;
int myid;
long strips;
long result;
long finalResult;

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
	double upper=0;
	double lower=0;
	for (i=0; i<iteration; i++)
	{
		xleft=start+i*size;
		xright=start+(i+1)*size;
		upper+=getUpperBound(xleft);
		lower+=getLowerBound(xright);
	}
	
}

void slave(myId)
{
	long strips_per_slave = strips/slaves;
	int extra_strips = strips_per_slave % slaves;
	double size = 1.0/strips;
	double start=0;
	if (myID <= extra_strips)
	{	
		//do 1 extra iteration
		start = myId*size*(strips_per_slave+1);
		slave_computation(start, strips_per_slave+1);
	}
	else 
	{
		start = myId*size*strips_per_slave + extra_strips;
		slave_computation(start, strips_per_slave);
	}
}