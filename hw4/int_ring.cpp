#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


float ring(int token, int N, int world_rank, int world_size){
  double tt = MPI_Wtime();
  //printf("Initial token: %d\n", token);
  // Find out rank, size
  //for(int i =0;i < N; i++){
  //int world_rank;
  //MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  //int world_size;
  //MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //int N = 2; // Set number of repeats for loop
  //int token = 0;
  //printf("Initial token: %d\n", token);
  // Receive from the lower process and send to the higher process. Take care
  // of the special case when you are the first process to prevent deadlock.
  for(int i =0;i < N; i++){
  if (world_rank != 0) {
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    token += world_rank;
    //printf("Process %d received token %d from process %d\n", world_rank, token,
    //       world_rank - 1);
    //MPI_Barrier(MPI_COMM_WORLD);
  }
  //else {
   // Set the token's value if you are process 0
    //token = 0;
  //}
  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
           MPI_COMM_WORLD);
  //MPI_Barrier(MPI_COMM_WORLD);
  // Now process 0 can receive from the last process. This makes sure that at
  // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
  // deadlock)
  if (world_rank == 0) {
     MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    //printf("Process %d received token %d from process %d\n", world_rank, token,
    //       world_size - 1);
    //MPI_Barrier(MPI_COMM_WORLD);
  }
}
tt = MPI_Wtime() - tt;
return tt;
}

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int token = 0;
  int N = 1000;
  float tt = ring(token,N,world_rank,world_size);
  if(!world_rank){
  printf("Time: %e ms\n",tt * 1000);
  printf("Latency: %e ms\n",tt/N * 1000);
  printf("Bandwidth: %e GB/s\n", (1e6*N)/tt/1e9);}
  MPI_Finalize();
}
  
