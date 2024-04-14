#include <stdio.h>

int main() {
    printf("Tamaño de un entero (int): %zu bytes\n", sizeof(int));
    printf("Tamaño de un caracter (char): %zu byte\n", sizeof(char));
    printf("Tamaño de un número de punto flotante (float): %zu bytes\n", sizeof(float));
    printf("Tamaño de un doble (double): %zu bytes\n", sizeof(double));
    printf("Tamaño de un short (short): %zu bytes\n", sizeof(short));
    printf("Tamaño de un long (long): %zu bytes\n", sizeof(long));
    
    return 0;
}