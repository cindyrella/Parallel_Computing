#include <omp.h>
#include <stdio.h>
#include <sys/time.h>

#define NUM_THREADS 2
int main(){


    struct timeval start, end;
    gettimeofday(&start, NULL);

    omp_set_num_threads(NUM_THREADS);

    double sum = 0;
    int n = 100000;
    double step = 1. / ((double) n);
    double x;
    #pragma omp parallel for schedule(static,1), private(x), reduction(+:sum)
     for (int i = 0; i < n ; ++i){
            x = (i + 0.5) * step;
            sum += 4. / (1. + x * x);
     }
    sum = sum * step;


    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
         end.tv_usec - start.tv_usec) / 1.e6;

    printf("El valor de pi es = %f\n",sum);
    printf("Tiempo de ejecucion = %f\n",delta);

}
