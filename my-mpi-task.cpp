#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{
  int n = 10;
  double a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  double b = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  int size, rank;
 
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int chunk_size = n / size;
  int remainder = n % size;

  int start = rank * chunk_size + (rank < remainder ? rank : remainder);
  int end = start + chunk_size + (rank < remainder ? 1 : 0);

  double local_c = 0.0;
  for (int i = start; i < end; i++) {
    local_c = local_c + a * b;
  }

  if (rank != 0) {
    MPI_Send(&local_c, 1, MPI_DOUBLE, 0, 555, MPI_COMM_WORLD);
  } else {
    double total_c = local_c;
    double temp_c;
    MPI_Status status;
    
    for (int i = 1; i < size; i++) {
      MPI_Recv(&temp_c, 1, MPI_DOUBLE, i, 555, MPI_COMM_WORLD, &status);
      total_c = total_c + temp_c;
    }
    
    std::cout << "Threads used: " << size << " | Final Dot Product c = " << total_c << std::endl;
  }
  
  MPI_Finalize();
  return 0;
}