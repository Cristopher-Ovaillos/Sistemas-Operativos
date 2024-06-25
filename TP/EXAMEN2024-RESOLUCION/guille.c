#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define FILE "desordenado.txt"
#define SIZE 1000


typedef struct {
    int inicio;
    int fin;
} arg_threads;


int arreglo[SIZE];
int arreglo_ordenado[SIZE];

// sem_t sem;


void something_wrong(int fd, const char *m){
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}


/* Compilar con: 
gcc -o thread ex01.c -lpthread */

void ordenar(int inicio, int fin){
    for (int i = inicio; i < fin; i++){
        for (int j = i+1; j < fin; j++){
            int aux = arreglo[i];
            if (aux > arreglo[j]){
                arreglo[i] = arreglo[j];
                arreglo[j] = aux;
            }
        }
    }
    
}

void *thread(void *arg) {
    arg_threads *data = (arg_threads *)arg; // Convertir el puntero void* al tipo arg_threads
    int inicio = data->inicio;
    int fin = data->fin;
    printf("Thread en rango [%d, %d] \n", inicio, fin);
    ordenar(inicio , fin);
}

void fusionar(){
    int a = 0;
    int b = 500;
    for(int i = 0; i < SIZE; i++){
        int elemA = arreglo[a];
        int elemB = arreglo[b];

        if (a < 500 && b < SIZE){
            if (elemA < elemB){
                a++;
                arreglo_ordenado[i] = elemA;
            } else {
                b++;
                arreglo_ordenado[i] = elemB;
            }

        } else{
            if (a < 500){ //b llego al final y quedaron los elementos de a, los copio directamente
               arreglo_ordenado[i] = elemA; 
               a++;
            } else { //a llego al final, quedaron ls elementos de b, los copio sin comparar directamente
                b++;
                arreglo_ordenado[i] = elemB;
            }
        }
    }
}
 

int main(){
    int in_fd = open(FILE, O_RDONLY);
    if (in_fd < 0){
        something_wrong(in_fd, "Error open");
    }

    int n;
    for(int i=0; i<SIZE; i++){
        read(in_fd, &n, sizeof(n));
        arreglo[i] = n;
        // printf("%d - nro: %i \n", i, n); // descomentar si quiere ver el array desordenado
    }
    close(in_fd);

    pthread_t tid[2];
    arg_threads args[2];

    args[0].inicio = 0;
    args[0].fin = 500;

    args[1].inicio = 500;
    args[1].fin = 1000;

    

    for(int i = 0; i < 2; i++){
        pthread_create(&tid[i], NULL, thread, (void *)&args[i]);
    }

    for(int i = 0; i < 2; i++){
        pthread_join(tid[i], NULL);
    }
 printf("Arreglo 1: \n");
    for(int i=0; i<500; i++){
        printf("- %i \n", arreglo_ordenado[i]); 
    }
     printf("Arreglo 2: \n");
    for(int i=500; i<SIZE; i++){
        printf("- %i \n", arreglo_ordenado[i]); 
    }
    
    
    //espera q terminen los hilos y luego fusiona
    // fusionar();
    //muestra por pantalla el array ordenado
   /*
    printf("Arreglo Ordenado: \n");
    for(int i=0; i<SIZE; i++){
        printf("- %i \n", arreglo_ordenado[i]); 
    }
   */
    

    return 0;
}
