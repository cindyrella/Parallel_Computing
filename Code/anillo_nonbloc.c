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
    MPI_Status send_status;
    MPI_Status recv_status;
    MPI_Request send_req;
    MPI_Request recv_req;

    for (int i = 0; i < iteraciones; ++i){ 
    
        if (rank == 0){
            MPI_Isend(&envio,1,MPI_DOUBLE,rank +1 ,i,MPI_COMM_WORLD,&send_req); 
            MPI_Irecv(&recibo,1,MPI_DOUBLE,size - 1,i,MPI_COMM_WORLD,&recv_req);
       }
        else{
            MPI_Irecv(&recibo,1,MPI_DOUBLE,rank - 1,i,MPI_COMM_WORLD,&recv_req);
            MPI_Isend(&envio,1,MPI_DOUBLE, (rank +1)%size ,i,MPI_COMM_WORLD,&send_req);
        }
        MPI_Wait(&recv_req,&recv_status);
        MPI_Wait(&send_req,&send_status);
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
