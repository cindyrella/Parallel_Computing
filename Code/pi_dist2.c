#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){ 

    MPI_Init(&argc, &argv);

    double start, finish, time;
    start = MPI_Wtime();

    int n = 100000;
    double step = 1./ ((double) n);
    double sum = 0;
    double x, total;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for( int i = rank; i < n; i += size){
        x = (i + 0.5) * step;
        sum += 4. / (1. + x * x);
    }

    MPI_Reduce( &sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
     
    finish = MPI_Wtime();  
    time = finish - start; 
    printf(" El tiempo del procesador %d es %f\n", rank, time); 
 
    if (rank == 0){ 
        total = total * step; 
        printf("El valor de pi es %f\n",total); 
    } 
 
    MPI_Finalize(); 
} 

