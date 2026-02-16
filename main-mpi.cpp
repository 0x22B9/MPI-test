#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{
  double a[10]={1,2,3,4,5,6,7,8,9,10};
  double b[10]={1,2,3,4,5,6,7,8,9,10};
  double c=0;

  int size, rank;
 
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i=rank*5; i<(rank+1)*5; i++) {
    c=c+a[i]*b[i];
  }

  // if (rank==0) {
  //   for (int i=0; i<5; i++) {
  //     c=c+a[i]*b[i];
  //   }
  // }

  // if (rank==1) {
  //   for (int i=5; i<10; i++) {
  //     c=c+a[i]*b[i];
  //   }
  // }

  // 0 поток отправляет с потоку 1
  // поток 1, получает данные от потока 0
  double c0;
  MPI_Status status;
  
  if (rank==0)
    MPI_Send(&c, 1, MPI_DOUBLE, 1, 555, MPI_COMM_WORLD);
  
  if (rank==1) {
    MPI_Recv(&c0, 1, MPI_DOUBLE, 0, 555, MPI_COMM_WORLD, &status);
    c=c+c0;
    std::cout << "rank=" << rank << " c=" << c << std::endl;
  }
  
  MPI_Finalize();
  return 0;
}
