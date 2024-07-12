#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);  //MPIの初期化
    int n =0;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  //現在のプロセスのランク
    MPI_Comm_size(MPI_COMM_WORLD, &size);  //プロセスの合計数


    //入力はランク0のプロセスが受け取る
    if (rank == 0) {
        
        extern int errno;
        errno =0; //errnoをリセット
        char buf[32], *endp;
        printf("n ="); fflush(stdout);
        if(scanf("%31[^\n]", buf) != 1) { //入力が正しくない場合のチェック
            fprintf(stderr, "Error in reading input\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  //全てのプログラムを異常終了させる
        }
        scanf("%*c");  //改行文字を読み飛ばす
        n =strtol(buf, &endp, 10);  //整数に変換
        if (*endp != '\0' || errno != 0 || n < 0) {  //エラーが起きた、または入力された数字が負の値の場合、エラーを返す
            fprintf(stderr, "illegal input number\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  //全てのプログラムを異常終了させる
        }
    }

    //nの値を全てのプロセスにブロードキャスト
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //円周率をもとめる計算
    double pi =0.0, pi_sum =0.0;
    const double h =2.0 / n;

    for (int i =rank; i < n; i += size) {
        const double x0 =i * h - 1.0;            //台形の左端
        const double x1 =(i + 1) * h - 1.0;      //台形の右端
        const double f0 =2.0 / (1.0 + x0 * x0);  //区間での積分計算に使用
        const double f1 =2.0 / (1.0 + x1 * x1);  //区間での積分計算に使用
        pi += 0.5 * (f0 + f1) * (x1 - x0);        //台形の面積を半分に
    }

    //各プロセスの計算結果を集める
    MPI_Reduce(&pi, &pi_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //最終的な結果はランク0が表示させる
    if (rank == 0) {
        printf("pi = %.16f\n", pi_sum);
    }

    MPI_Finalize();  //MPIプログラムの終了宣言
    return 0;
}
