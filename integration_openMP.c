#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

long long wall_clock_time()
{
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        return (long long)(tp.tv_nsec + (long long)tp.tv_sec*1000000000ll);
}

long getUpperBound(double x, double scale)
{
        float y;
        y = 4.0/(1+pow(x,2))*scale;
        if (y-(long)y > 0)
                y = (long)y+1;
        return (long)y;
}

long getLowerBound(double x, double scale)
{
        float y;
        y = 4.0/(1+pow(x,2))*scale;
        return (long)y;
  
½nt main(int argc, char **argv)
{
        long n;
        int i;
        double size;
        double xleft =0;
        double xright=0;
        long sumUpper=0;
        long sumLower=0;
        long long before, after;
        if (argc >=2)
                n = atol(argv[1]);                                                                                                                                           
        else                                                                                                                                                                 
                n = 10;                                                                                                                                                      
                                                                                                                                                                             
        before = wall_clock_time();                                                                                                                                          
        size = 1.0/n;                                                                                                                                                        
        #pragma omp parallel for private (xleft, xright) reduction (+:sumUpper,sumLower)                                                                                     
        for (i=0; i<n; i++)                                                                                                                                                  
        {                                                                                                                                                                    
                xleft = (double)i*size;                                                                                                                                      
                xright=(double)(i+1)*size;                                                                                                                                   
                sumUpper+= getUpperBound(xleft, n);                                                                                                                          
                sumLower+= getLowerBound(xright, n);                                                                                                                         
        }                                                                                                                                                                    
                                                                                                                                                                             
        double result= (double)(sumUpper+sumLower)/2.0*(size*size);                                                                                                          
        after = wall_clock_time();                                                                                                                                           
        printf("result: %.8lf \n", result);                                                                                                                                  
        int threads;                                                                                                                                                         
        #pragma omp parallel
        {
        threads = omp_get_num_threads();                                                                                                                                     
        }
        printf("The integration took %2.4f seconds using %d threads and %li strips \n", ((float)(after-before))/1000000000,threads,n);
        return 0;                                             
}
