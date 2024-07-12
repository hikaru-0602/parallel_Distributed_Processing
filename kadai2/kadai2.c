#include <stdio.h>
#include <pthread.h>

#define N_THREADS (4)
static long num_rects = 2 * 1000 * 1000 * 1000;

typedef struct {
    int start_idx;
    int end_idx;
    double sum;
} ThreadData;

void* calc_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double mid, height, width = 1.0 / (double)num_rects;
    data->sum = 0.0;

    for (int i = data->start_idx; i < data->end_idx; ++i) {
        mid = (i + 0.5) * width;
        height = 4.0 / (1.0 + mid*mid);
        data->sum += height;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[N_THREADS];
    ThreadData thread_data[N_THREADS];
    int rects_per_thread = num_rects / N_THREADS;
    double sum = 0.0;

    for (int i = 0; i < N_THREADS; ++i) {
        thread_data[i].start_idx = i * rects_per_thread;
        thread_data[i].end_idx = (i == N_THREADS-1) ? num_rects : (i+1) * rects_per_thread;
        pthread_create(&threads[i], NULL, calc_pi, &thread_data[i]);
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        sum += thread_data[i].sum;
    }

    double area = (1.0 / (double)num_rects) * sum;
    printf("Computed pi = %f\n", area);

    return 0;
}
