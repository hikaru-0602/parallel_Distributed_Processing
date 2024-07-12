#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]){
    extern int errno;
    char buf[32], *endp;
    int n;
    
    printf("n = "); fflush(stdout);
    scanf("%31[^\n]%*[^\n]", buf); scanf("%*c");
    n = strtol(buf, &endp, 0); //整数に変換
    if (*endp != '\0' || errno != 0 || n < 0) {  //エラーが起きた、または入力された数字が負の値の場合、エラーを返す
        fprintf(stderr, "illegal input number\n");
        return -1;
    } 

    double pi = 0.0;
    const double h = 2.0 / n; //-1から１の幅をn等分

    for (int i = 0; i < n; i += 1) {
        const double x0 = i * h - 1.0;            //台形の左端
        const double x1 = (i + 1) * h - 1.0;      //台形の右端
        const double f0 = 2.0 / (1.0 + x0 * x0);  //区間での積分計算に使用
        const double f1 = 2.0 / (1.0 + x1 * x1);  //区間での積分計算に使用
        pi += 0.5 * (f0 + f1) * (x1 - x0);        //台形の面積を半分に
    }

    printf("pi = %.16f\n", pi);
    return 0;
}