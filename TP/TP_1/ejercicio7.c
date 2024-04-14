#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


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


int encontrarPrimosPadre() {
    
    for (int i = 0; i <=1000; i++) {
        if (esPrimo(i)) {
            printf("%d\n", i);  
        }
    }
return 0;
}
int encontrarPrimosHijo() {
    
    for (int i = 1000; i <= 5000; i++) {
        if (esPrimo(i)) {
            printf("%d\n", i);  
        }
    }
return 0;
}


void main(void) {
    int pid = fork();  // Crear un proceso hijo

    if (pid == 0) {  // Proceso hijo
    printf(" Empezo  el hijo.\n");
    encontrarPrimosHijo();
    printf(" ELimino el hijo.\n");
     exit(0);
          
    } else  {  // Proceso padre
        printf(" Proceso padre iniciado.\n");
        encontrarPrimosPadre();
        printf(" Proceso padre finalizado\n");
        kill(pid,SIGKILL);
        printf(" ELimino el hijo.\n");
         exit(0);
    } 

    
}
