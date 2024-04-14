#include <stdio.h>

int main() {
     printf("%d\n", 20/3);//6
    printf("%f\n", 20/3);//06.000000
    printf("%f\n", 20/3.);//06.666667
    printf("%d\n", 10%3);//1
    printf("%d\n", 3.1416);// 3 o tambien si saco el signo y exponente 
    printf("%f\n", (double)20/3);// 6.666667 porque si bien son 8 bytes de double luego usa %f de float
    printf("%f\n", (int)3.1416);//03.000000
    printf("%d\n", (int)3.1416);//3
    
    return 0;
}