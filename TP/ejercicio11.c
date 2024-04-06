#include <stdio.h>

int main() {
    int resultado = 100000 * 100000;
    printf("El resultado de la multiplicacion es: %d\n", resultado);
    printf("El tamaño de los ints en este sistema es: %lu bytes\n", sizeof(int));
    /*
    el rango es de -2.147-483.648 hasta 2.147.483.647

    es numero numero muy grande por lo tanto desborsa
    */
    return 0;
}