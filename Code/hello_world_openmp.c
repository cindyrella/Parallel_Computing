#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]){
    #pragma omp parallel
    {
      int tid = omp_get_thread_num();
      int nthreads = omp_get_num_threads();
      printf("Hola desde el thread %d de %d en total\n", tid, nthreads);
    }
}
