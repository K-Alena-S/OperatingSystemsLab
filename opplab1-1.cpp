#include <iostream>
#include <mpi.h>
#include <cmath>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#define N 16062
#define t 0.0001

int main(int argc, char* argv[]) {
    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double starttime, endtime;
    starttime = MPI_Wtime();

    int lrows = N / size;
    int excess = N % size;

    int* d = new int[size];
    int* p = new int[size];

    for (int  i = 0; i < size; i++){
        p[i] = (i < excess) ? lrows + 1 : lrows;
        if (i == 0) d[i] = 0;
        else d[i] = d[i-1] + p[i-1];
    }

    lrows += (rank < excess) ? 1 : 0;

    double* b = new double[N];
    for (int i = 0; i < N; i++) b[i] = double(N + 1);

    double x[N];
    for (int i = 0; i < N; i++) x[i] = 0.0;

    double* A = new double[N * N];
    for (int i = 0; i < lrows; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = 1.0;
        }
        int index = i * N + d[rank] + i;
        A[index] = 2.0;
    }

    double* part_x = new double[lrows];
   
    double B  = (N + 1) sqrt(N);
    double norm = 0.0;
    do{
        double res = 0.0;
        for(int i = 0; i < lrows; i++){
            double a = 0.0;
            for (int j = 0; j < N; j++) a += A[i*N+j] * x[j];
                a -= b[i];
                part_x[i] = x[d[rank]+i] - t * a;
                res += a*a;
            }
            MPI_Allgatherv(part_x, lrows, MPI_DOUBLE, x, p, d, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Allreduce(&res, &norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

            norm = sqrt(norm);
        } while ((norm / B) >= 0.0001);

    MPI_Barrier(MPI_COMM_WORLD);

    endtime = MPI_Wtime();
    if (rank == 0)  std::cout << "Time:"<< endtime - starttime <<"\n";

    for (int k = 0; k < size; k++) {
        if (rank == k) {
            std::cout << "rank: " << rank << "\n";
            for (int i = 0; i < N; i++) {
                std::cout << x[i] << " ";
            }
            std::cout << "\n ";

        }
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)  std::cout << "Time:"<< endtime - starttime <<"\n";

    MPI_Finalize();

    return 0;
}
