#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>//時間を計測する

#define NLOOP	(200)
#define N	(16384)
#define M	(16384)
#define ALIVE	(1)
#define DEAD	(0)
#define N_THREADS (100)//スレッドの数を指定。これをいろんな数字にして最適解を求める。１００がめちゃくちゃ早い


typedef int Grid[N + 2][M + 2];

void computeNextGen(const Grid * pCur, Grid * pNext); //プロトタイプ宣言。使ってみた

//スレッドの処理を格納した構造体
typedef struct {
    int start;//開始位置
    int end;//終了位置
    const Grid *pCur;
    Grid *pNext;
} ThreadData;

//スレッドの内容を定義
void* computeNextGenThread(void* arg){
    ThreadData* region = (ThreadData*)arg;
    computeNextGen(region->pCur, region->pNext);
    pthread_exit(0);
    return NULL;
}

void computeNextGen(const Grid * pCur, Grid * pNext)//次世代を計算する関数
{
	int i, j;

    //周りのセルの状態を確かめる
	for (i = 1; i <= N/N_THREADS; ++i) {
		for (j = 1; j <= M; ++j) {
			int count = 0;

			/* 左斜め上 */
			if ((*pCur)[i - 1][j - 1] == ALIVE)
				count++;
			/* 上 */
			if ((*pCur)[i - 1][j] == ALIVE)
				count++;
			/* 右斜め上 */
			if ((*pCur)[i - 1][j + 1] == ALIVE)
				count++;
			/* 左 */
			if ((*pCur)[i][j - 1] == ALIVE)
				count++;
			/* 右 */
			if ((*pCur)[i][j + 1] == ALIVE)
				count++;
			/* 左斜め下 */
			if ((*pCur)[i + 1][j - 1] == ALIVE)
				count++;
			/* 下 */
			if ((*pCur)[i + 1][j] == ALIVE)
				count++;
			/* 右斜め下 */
			if ((*pCur)[i + 1][j + 1] == ALIVE)
				count++;

            //周りのセルの生きてる数で次世代計算
			if (count <= 1 || count >= 4)
				(*pNext)[i][j] = DEAD;
			else if ((*pCur)[i][j] == ALIVE &&
				 (count == 2 || count == 3))
				(*pNext)[i][j] = ALIVE;
			else if ((*pCur)[i][j] == DEAD && count == 3)
				(*pNext)[i][j] = ALIVE;
			else
				(*pNext)[i][j] = DEAD;
		}
	}
}



int main(int argc, char *argv[]){
    static Grid g[2];
    int i, j, n, cur = 0;
    pthread_t threads[N_THREADS];
    ThreadData thread_data[N_THREADS];

    printf("\033[2J");    /* clear screen */

    //初期世代設定
    for (i = 0; i <= N + 1; ++i) {
        for (j = 0; j <= M + 1; ++j) {
            g[0][i][j] = random() & 1;
        }
        printf("Initializing g[%06d]...\r", i);
    }

    for (n = 0; n < NLOOP; n++) {
        printf("\033[2J");    /* clear screen */
        printf("n = %d\n", n);

        for (i = 1; i <= N >> 9; ++i) {
            for (j = 1; j <= M >> 6; ++j) {
                printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
            }
            printf("\n");
        }

        // スレッドの数にグリッド分割して計算を行う
        int rects_per_thread = N / N_THREADS;
        for (i = 0; i < N_THREADS; ++i) {
            thread_data[i].start = i * rects_per_thread;
            thread_data[i].end = (i == N_THREADS - 1) ? N : (i + 1) * rects_per_thread;
            thread_data[i].pCur = &g[cur];
            thread_data[i].pNext = &g[(cur + 1) & 1];
            pthread_create(&threads[i], NULL, computeNextGenThread, &thread_data[i]);
        }

        // スレッドの終了を待つ
        for (i = 0; i < N_THREADS; ++i) {
            pthread_join(threads[i], NULL);
        }


        cur = (cur + 1) & 1;
    }

    return 0;
}