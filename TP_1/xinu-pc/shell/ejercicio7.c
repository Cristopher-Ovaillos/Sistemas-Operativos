#include <xinu.h>
#include "shprototypes.h"


int esPrimo(int numero)
{
    int valPrimo = 1; // no hay booleanos en c asi que supondremos que es primo
    int salirBucle = 1;
    int i = 2;
    // el numero primo es un numero natural mayor que 1
    if (numero <= 1)
    {
        valPrimo = 0;
    }
    else
    {
 //el numero de divisores que puede tener un numero es el resultado de la raiz cuadrada del numero
 
        while (salirBucle && (i * i) <= numero)
        {
            if ((numero % i) == 0)
            {
                valPrimo = 0;
                salirBucle = 0;
            }

            i = i + 1;
        }
    }

    return valPrimo;
}

void obtenerPrimos(int inicio, int fin) {
    for (int i = inicio; i <= fin; i++) {
        if (esPrimo(i)) {
            printf("%d ", i);
        }
    }
}

void procesoHijo() {
    printf("Proceso hijo iniciado.\n");
    obtenerPrimos(1000,5000);
    printf("Proceso hijo finalizado.\n");
}

void procesoPadre() {
    printf("Proceso padre iniciado.\n");
    

    // Creo el proceso hijo con lo que tiene que hacer
    int pid = create(procesoHijo, 4096, 20, "Hijo", 0);
    resume(pid);
    obtenerPrimos(0,1000);
    // Finalizo el proceso hijo
    kill(pid);
    printf("Proceso hijo finalizado por el padre.\n");
    printf("Proceso padre finalizado.\n");
    exit();
}

void ejercicio7(void) {
    
    procesoPadre();

}