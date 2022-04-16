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

    for (int i = 0; i < size; i++) {
        p[i] = (i < excess) ? lrows + 1 : lrows;
        if (i == 0) d[i] = 0;
        else d[i] = d[i - 1] + p[i - 1];
    }

    lrows += (rank < excess) ? 1 : 0;

    double* b = new double[lrows];
    for (int i = 0; i < lrows; i++) b[i] = double(N + 1);

    double x[p[0]];
    for (int i = 0; i < p[0]; i++) x[i] = 0.0;

    double* A = new double[N * lrows];
    for (int i = 0; i < lrows; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = 1.0;
        }
        int index = i * N + d[rank] + i;
        A[index] = 2.0;
    }

    double B  = (N + 1) * sqrt(N);
    double norm = 0.0;

    double* y = new double[p[0]];
    for (int i = 0; i < p[0]; i++) y[i] = 0.0;
    double* c = new double[lrows];

    do {
        for (int i = 0; i < lrows; i++) c[i] = 0.0;
        double res = 0.0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < lrows; j++) {
                    for (int k = 0; k < p[(i + rank) %size]; k++) {
                        c[j] += A[N * j + d[(i + rank) %size] + k] * y[k];
                    }
            }

            MPI_Sendrecv(x, p[rank], MPI_DOUBLE, (rank  + 1) % size, 123, y, p[0],
                MPI_DOUBLE, (rank+size -1)%size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int m = 0; m < p[(rank+i)%size]; m++) x[m] = y[m];

        }
        for (int i = 0; i < lrows; i++) {
            c[i] -= b[i];
            x[i] -= t * c[i];
            res += c[i] * c[i];
        }
        for (int i = 0; i < p[0]; i++) y[i] = x[0];
        MPI_Allreduce(&res, &norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    } while ((norm / B) >= 0.00001);
  
    MPI_Barrier(MPI_COMM_WORLD);
    endtime = MPI_Wtime();

    for (int k = 0; k < size; k++) {
        if (rank == k) {
            std::cout << "rank: " << rank << "\n";
            for (int i = 0; i < lrows; i++) {
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
