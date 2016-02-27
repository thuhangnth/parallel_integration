#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

long getUpperBound(double x, double scale)
{
        float y;
        y = 4.0/(1+pow(x,2))*scale;
        //printf ("minus: %f \n", y-(long)y);
        if (y-(long)y > 0)
                y = (long)y+1;
        return (long)y;
}
{
        float y;
        y = 4.0/(1+pow(x,2))*scale;
        return (long)y;
}
int main(int argc, char **argv)
{
        long n;
        int i;                                                
        double size;                                          
        double xleft =0;                                      
        double xright=0;                                      
        long sumUpper=0;                                      
        long sumLower=0;                                      
        long upper, lower;                                    
        if (argc >=2)                                         
                n = atol(argv[1]);                            
        else                                                  
                n = 10;                                       
        size = 1.0/n;                                         
                                                              
        #pragma omp parallel for private (i, upper, lower, xleft, xright) reduction (+:sumUpper,sumLower)
        for (i=0; i<n; i++)                                   
        {       
                xleft = (double)i*size;                       
                xright=(double)(i+1)*size;                    
                upper = getUpperBound(xleft, n);
                lower = getLowerBound(xright, n);             
                                                              
                sumUpper += upper;                            
                sumLower += lower;                            
        }                                                     
                                                              
        printf("upper: %li, lower :%li \n", sumUpper, sumLower);
        double result= (double)(sumUpper+sumLower)/2*(size*size);
        printf("result: %.8lf \n", result);
        return 0;                                             
}
