/*
 * The Life Game for 2023 FUN class
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>//時間を計測する

#define NLOOP	(200)
#define N	(16384)
#define M	(16384)
#define ALIVE	(1)
#define DEAD	(0)

double elapsed_time=0.0;  //n世代目の計算の時間。mainで使用する

typedef int Grid[N + 2][M + 2];

void computeNextGen(const Grid * pCur, Grid * pNext)
{
	int i, j;

	for (i = 1; i <= N; ++i) {
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
			/* 右斜めした */
			if ((*pCur)[i + 1][j + 1] == ALIVE)
				count++;

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

int main(int argc, char *argv[])
{
	static Grid g[2];
	int i, j, n, cur = 0;

	printf("\033[2J");	/* clear screen */
	for (i = 0; i <= N + 1; ++i) {
		for (j = 0; j <= M + 1; ++j) {
			g[0][i][j] = random() & 1;
		}
		printf("Initializing g[%06d]...\r", i);
	}

	for (n = 0; n < NLOOP; n++) {
		clock_t start_time, end_time; //時間の開始と終了
		printf("\033[2J");	/* clear screen */
		printf("n = %d\n", n);
		printf("時間は%.6f\n",elapsed_time);

		start_time = clock(); //時間計測開始
		for (i = 1; i <= N >> 9; ++i) {
			for (j = 1; j <= M >> 6; ++j) {
				printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
			}
			printf("\n");
		}
		computeNextGen(&g[cur], &g[(cur + 1) & 1]);
		end_time = clock(); // 終了時刻を記録
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // 経過時間を計算
		cur = (cur + 1) & 1;
	}

	return 0;
}
