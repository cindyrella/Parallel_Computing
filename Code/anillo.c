#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){ 

    MPI_Init(&argc, &argv);

    double start, finish, time;
    start = MPI_Wtime();

    int iteraciones = 10;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    double envio = rank;
    double recibo = size;
    MPI_Status status;

    for (int i = 0; i < iteraciones; ++i){ 
    
        if (rank == 0){
            MPI_Send(&envio,1,MPI_DOUBLE,rank +1 ,i,MPI_COMM_WORLD); 
            MPI_Recv(&recibo,1,MPI_DOUBLE,size - 1,i,MPI_COMM_WORLD,&status);
       }
        else{
            MPI_Recv(&recibo,1,MPI_DOUBLE,rank - 1,i,MPI_COMM_WORLD,&status);
            MPI_Send(&envio,1,MPI_DOUBLE, (rank +1)%size ,i,MPI_COMM_WORLD);
        }
        envio = recibo;

   }

    MPI_Barrier(MPI_COMM_WORLD); 
    if( rank == 0){
        finish = MPI_Wtime();  
        time = finish - start; 
        printf(" El tiempo total %d es %f\n", rank, time); 
    }
    MPI_Finalize(); 
} 
