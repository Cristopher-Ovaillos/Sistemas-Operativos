#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Función para verificar si un número es primo
int esPrimo(int numero) {
    if (numero <= 1) return 0;  // Los números menores o iguales a 1 no son primos

    for (int i = 2; i * i <= numero; i++) {
        if (numero % i == 0) return 0;  // Si el número es divisible por algún otro número, no es primo
    }
    return 1;  // Si no se encontraron divisores, el número es primo
}

// Función para encontrar números primos en un rango dado y contar cuántos encontró
int encontrarPrimos(int inicio, int fin) {
    int contador = 0;
    for (int i = inicio; i <= fin; i++) {
        if (esPrimo(i)) {
            contador++;
            printf("%d\n", i);  // Imprimir el número primo encontrado
        }
    }
    return contador;
}

int main() {
    int pid = fork();  // Crear un proceso hijo

    if (pid == 0) {  // Proceso hijo
        printf("Proceso hijo iniciado.\n");
        printf("Los números primos del 1000 al 5000 son:\n");
        int primosEncontrados = encontrarPrimos(1000, 5000);
        printf("Proceso hijo finalizado. Se encontraron %d números primos.\n", primosEncontrados);
        exit(0);  // Finalizar el proceso hijo
    } else if (pid > 0) {  // Proceso padre
        printf("Proceso padre iniciado.\n");
        printf("Los números primos del 0 al 1000 son:\n");
        int primosEncontrados = encontrarPrimos(0, 1000);
        printf("Proceso padre finalizado. Se encontraron %d números primos.\n", primosEncontrados);
        
        printf("Proceso hijo ha terminado.\n");
    } else {  // Error al crear el proceso hijo
        perror("Error al crear el proceso hijo.\n");
        return 1;
    }

    return 0;
}