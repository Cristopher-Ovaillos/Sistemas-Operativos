#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


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


int encontrarPrimos(int inicio, int fin) {
    
    for (int i = inicio; i <= fin; i++) {
        if (esPrimo(i)) {
            printf("%d ", i);  
        }
    }
return 0;
}

int main() {
    int pid = fork();  // Crear un proceso hijo

    if (pid == 0) {  // Proceso hijo
        printf("Proceso hijo iniciado.\n");
        encontrarPrimos(1000, 5000);
        printf("Proceso hijo finalizado > fin\n");
        exit(0);  
    } else  {  // Proceso padre
        printf("Proceso padre iniciado.\n");
        
        encontrarPrimos(0, 1000);
        printf("Proceso padre finalizado\n");
        kill(pid,SIGKILL);
        printf("ELimino el hijo.\n");
    } 
    
    return 0;
}