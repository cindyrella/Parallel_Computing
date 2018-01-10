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

    MPI_Status status;
    for( int i = rank; i < n; i += size){
        x = (i + 0.5) * step;
        sum += 4. / (1. + x * x);
    }
    if (rank == 0){
        total = sum;
        for (int j = 1; j < size; ++j){
            MPI_Recv(&sum,1,MPI_DOUBLE,j,0,MPI_COMM_WORLD,&status);
            total += sum;
        }
    } 
    else{
        MPI_Send(&sum,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
   } 
    
    finish = MPI_Wtime();  
    time = finish - start; 
    printf(" El tiempo del procesador %d es %f\n", rank, time); 
 
    if (rank == 0){ 
        total = total * step; 
        printf("El valor de pi es %f\n",total); 
    } 
 
    MPI_Finalize(); 
} 

