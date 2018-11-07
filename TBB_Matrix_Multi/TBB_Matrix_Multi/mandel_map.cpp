/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 */

/*

   Author: Marco Aldinucci.
   email:  aldinuc@di.unipi.it
   marco@pisa.quadrics.com
   date :  15/11/97

Modified by:

****************************************************************************
 *  Author: Dalvan Griebler <dalvangriebler@gmail.com>
 *
 *  Copyright: GNU General Public License
 *  Description: This program simply computes the mandelbroat set.
 *  File Name: mandel.cpp
 *  Version: 1.0 (25/05/2018)
 *  Compilation Command: make
 ****************************************************************************
*/

#include "pch.h"
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <chrono>
#include <tbb/tbb.h>

#define DIM 1000
#define ITERATION 30000

using namespace tbb;
using namespace std;
using namespace std::chrono;

affinity_partitioner ap;

double init_a=-2.125,init_b=-1.5,range=3.0;
double step,im;
unsigned char *M;
int dim = DIM, niter = ITERATION;
int threadCount = 1;

//Extra Classes
class MapOperation{
public:
    void operator()(const blocked_range<int>& range) const{
        double a,b,a2,b2,cr;
        for(int j = range.begin(); j < range.end(); j++)
        {
            a=cr=init_a+step*j;
            b=im;
            int k=0;
            for (k=0; k<niter; k++)
            {
                a2=a*a;
                b2=b*b;
                if ((a2+b2)>4.0) break;
                b=2*a*b+im;
                a=a2-b2+cr;
            }
            M[j]= (unsigned char) 255-((k*255/niter));
        }
    }
    MapOperation(){}
};

double diffmsec(struct timeval  a,  struct timeval  b) {
    long sec  = (a.tv_sec  - b.tv_sec);
    long usec = (a.tv_usec - b.tv_usec);

    if(usec < 0) {
        --sec;
        usec += 1000000;
    }
    return ((double)(sec*1000)+ (double)usec/1000.0);
}



int main(int argc, char **argv) {

    // stats
    struct timeval t1,t2;
    int r, retries=5;
    double avg=0, var, * runs;


    if (argc<3) {
        printf("Usage: seq size niterations\n\n\n");
    }
    else {
        dim = atoi(argv[1]);
        niter = atoi(argv[2]);
        step = range/((double) dim);
        retries = atoi(argv[3]);
    }
    task_scheduler_init(1);

    runs = (double *) malloc(retries*sizeof(double));

    M = (unsigned char *) malloc(dim);

    printf("Mandebroot set from (%g+I %g) to (%g+I %g)\n",
           init_a,init_b,init_a+range,init_b+range);
    printf("resolution %d pixel, Max. n. of iterations %d\n",dim*dim,ITERATION);

    step = range/((double) dim);

#if !defined(NO_DISPLAY)
    //SetupXWindows(dim,dim,1,NULL,"Sequential Mandelbroot");
#endif
    for (r=0; r<retries; r++)
    {
        // Start time
        //gettimeofday(&t1,NULL);
		steady_clock::time_point t3 = steady_clock::now();
        for(int i=0; i<dim; i++)
        {
            im=init_b+(step*i);
            parallel_for(blocked_range<int>(0, dim),MapOperation(), ap);
#if !defined(NO_DISPLAY)
            //ShowLine(M,dim,i);
#endif
        }
        // Stop time
		steady_clock::time_point t4 = steady_clock::now();
        //gettimeofday(&t2,NULL);
		duration<double> time_span = duration_cast<duration<double>>(t4 - t3);
		runs[r] = time_span.count();
		avg += runs[r];//diffmsec(t4,t3);
        printf("Run [%d] DONE, time= %f (ms)\n",r, runs[r]);
    }
    avg = avg / (double) retries;
    var = 0;
    for (r=0; r<retries; r++) {
        var += (runs[r] - avg) * (runs[r] - avg);
    }
    var /= retries;
    printf("Average on %d experiments = %f (ms) Std. Dev. %f\n\nPress a key\n",retries,avg,sqrt(avg));

#if !defined(NO_DISPLAY)
    getchar();
    //CloseXWindows();
#endif

    return 0;
}
