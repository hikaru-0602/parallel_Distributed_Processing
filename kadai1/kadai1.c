#include <stdio.h>
#include <pthread.h>

#define N 6

int A[N][N] = {
    {1, 2, 3, 4, 5, 6},
    {2, 3, 4, 5, 6, 7},
    {3, 4, 5, 6, 7, 8},
    {4, 5, 6, 7, 8, 9},
    {5, 6, 7, 8, 9, 0},
    {6, 7, 8, 9, 0, 1}
};

int B[N][N] = {
    {9, 8, 7, 6, 5, 4},
    {8, 7, 6, 5, 4, 3},
    {7, 6, 5, 4, 3, 2},
    {6, 5, 4, 3, 2, 1},
    {5, 4, 3, 2, 1, 0},
    {4, 3, 2, 1, 0, 9}
};

int C[N][N];

void *matrixmulti(void *row) {
    int *r = (int *)row;
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            C[*r][j] += A[*r][k] * B[k][j];
        }
    }
    pthread_exit(0);
}

int main() {
    pthread_t threads[N];
    int rowIndices[N];

    for (int i = 0; i < N; i++) {
        rowIndices[i] = i;
        pthread_create(&threads[i], NULL, matrixmulti, &rowIndices[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}