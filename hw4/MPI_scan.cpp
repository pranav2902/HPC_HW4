#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_arr(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  int i;
  printf("The elements of random array are: ");
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand()/ (float)RAND_MAX);
    printf("%f \n", rand_nums[i]);
  }
  return rand_nums;
}
float * partition_sum(float *array, int num_elements) {
  int i;
  for (i = 1; i < num_elements; i++) {
    array[i] += array[i-1];
  }
  return array;
}
float * add_offsets(float *array, int num_elements, float *offset, int rank){
  float off = 0;
  for (int j = 0; j<rank; j++){
    off += offset[j];
  }
  int i;
  for (i = 0; i < num_elements; i++) {
    array[i] += off;
  }
  return array;
}
int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements_per_proc\n");
    exit(1);
  }
  int num_elements_per_proc = atoi(argv[1]);
  // Seed the random number generator to get different results each time
  srand(time(NULL));
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  // Create a random array of elements on the root process. Its total
  // size will be the number of elements per process times the number
  // of processes
  float *rand_arr = NULL;
  if (world_rank == 0) {
    rand_arr = create_rand_arr(num_elements_per_proc * world_size);
  }
  // For each process, create a buffer that will hold a subset of the entire
  // array

  float *sub_rand_arr = (float *)malloc(sizeof(float) * num_elements_per_proc);
  // Scatter the random numbers from the root process to all processes in
  // the MPI world

  MPI_Scatter(rand_arr, num_elements_per_proc, MPI_FLOAT, sub_rand_arr,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
  float * local_sums = partition_sum(sub_rand_arr, num_elements_per_proc);
  // Gather all partial averages down to all the processes

  float *sub_offsets = (float *)malloc(sizeof(float) * world_size);
  assert(sub_offsets != NULL);
  MPI_Allgather(&local_sums[num_elements_per_proc - 1], 1, MPI_FLOAT, sub_offsets, 1, MPI_FLOAT, MPI_COMM_WORLD);
  float *partition_with_offset = add_offsets(local_sums, num_elements_per_proc, sub_offsets, world_rank);
MPI_Barrier(MPI_COMM_WORLD);
  //printf("\nElements after scan are: ");
for (int i = 0; i < num_elements_per_proc; i++){
        printf("Elements after scan are: %f \n", partition_with_offset[i]);
  }
  if (world_rank == 0) {
    free(rand_arr);
    }
  free(sub_offsets);
  free(sub_rand_arr);
  //MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
