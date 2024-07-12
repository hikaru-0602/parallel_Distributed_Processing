#include <stdio.h>
#include "mpi.h"
#define NDATA 32
int main(int argc, char **argv)
{
int rank, nprocs, mysum=0, result=0, namelen;
char name[MPI_MAX_PROCESSOR_NAME];
int data[NDATA] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
31, 32
};
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Get_processor_name(name, &namelen);
MPI_Barrier(MPI_COMM_WORLD);
int x = NDATA / nprocs;
int low = rank * x;
int high = low + x;
for(int i = low; i < high; i++) {
mysum += data[i];
}
printf("%s(%d/%d): value = %d¥n", name, rank, nprocs, mysum);
MPI_Reduce(&mysum, &result, 1, MPI_INT,
MPI_SUM, 0, MPI_COMM_WORLD);
if(rank == 0)
printf("The sum is %d.¥n", result);
MPI_Finalize();
return 0;
}