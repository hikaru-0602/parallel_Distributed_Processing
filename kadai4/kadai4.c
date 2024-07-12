/*
 * 並列分散処理 第4回課題
 * Nov. 14th, 2022
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N (29) //クイーンの最大数

//解を探索する関数
int check_and_set(unsigned int mat[], int n, int row, int col)
{
	int i, j, c = 0;

	//水平方向 
	for (j = col - 1; j >= 0; j--) {
		if (mat[j] & (1U << row)) { //col行目を順に当てはめ、その列にクイーンがあるか評価
			return c;
		}
	}

	//斜め上方向
	for (i = row - 1, j = col - 1; j >= 0 && i >= 0; j--, i--) {
		if (mat[j] & (1U << i)) {
			return c;
		}
	}

	//斜め下方向
	for (i = row + 1, j = col - 1; j >= 0 && i < n; j--, i++) {
		if (mat[j] & (1U << i)) {
			return c;
		}
	}

	//配置できる場合そこに配置する
	mat[col] = 1 << row;

	if (col == n - 1) { //クイーンが全て配置された場合
		c++;
	} else {
		//残りの列にクイーンを配置
		for (i = n - 1; i >= 0; i--)
			c += check_and_set(mat, n, i, col + 1);
	}
	mat[col] = 0U;

	return c;
}

int main(int argc, char *argv[])
{
	unsigned int mat[MAX_N], n;
	int i, count = 0;

	if (argc < 2) { //コマンドラインで引数が無い場合
		fprintf(stderr, "Usage: %s number\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]); //引数を変換し、nに代入
	if ((n < 2) || (n > MAX_N)) { //クイーンパズルが成立するための最低限の条件
		fprintf(stderr, "You should specify a number "
			"between 2 and %d.\n", MAX_N);
		return -1;
	}

	for (i = 0; i < n; i++)
		mat[i] = 0U; //matの初期化
	
	#pragma omp parallel for private(mat, i) reduction(+:count) //Open MP構文で並列化
	for (i = n - 1; i >= 0; i--)
		count += check_and_set(mat, n, i, 0);
	printf("Total answer for %d x %d = %d\n", n, n, count);

	return 0;
}