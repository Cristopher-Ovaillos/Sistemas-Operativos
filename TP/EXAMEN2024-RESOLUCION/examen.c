#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define ARCHIVO "desordenado.txt"
#define NUM_ELEMENTOS 1000

int numeros[NUM_ELEMENTOS];
int numerosF[NUM_ELEMENTOS];
typedef struct
{
    int inicio;
    int fin;
} arg_thread;

void something_wrong(int fd, const char *m)
{
    if (fd >= 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}
// void ord(int inicio, int fin,int arreglo[] ){
void ord(int inicio, int fin)
{
    for (int i = inicio; i < fin; i++)
    {
        for (int j = i + 1; j < fin; j++)
        {
            int aux = numeros[i];
            if (aux > numeros[j])
            {
                numeros[i] = numeros[j];
                numeros[j] = aux;
            }
        }
    }
}
void *intermedio(void *strucArg)
{
    arg_thread *args = (arg_thread *)strucArg;
    ord(args->inicio, args->fin);

    return NULL;
}
void fusionar() {
      // arreglo ordenado segun su rango ahora solo comparo y armo uno nuevo
    int seg = 500;
    int a=0;
    int primAux, segAux;
    for (int i = 0; i < NUM_ELEMENTOS; i++)
    {
        primAux = numeros[i];
        segAux = numeros[seg];

        if (primAux < segAux)
        {
            numerosF[a] = primAux;
            a++;
            numerosF[a] = segAux;
             a++;
        }
        else
        {
            numerosF[a] = segAux;
             a++;
            numerosF[a] = primAux;
             a++;
        }

        // Asegurar que seg no se salga de los límites del arreglo
        seg++;
        if (seg >= NUM_ELEMENTOS)
        {
            break; // Terminar si seg excede el tamaño del arreglo
        }
    }
}


int main()
{
    pthread_t hilo1, hilo2;
    arg_thread strucAg[2];
    int in_fd = open(ARCHIVO, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong(in_fd, "Error open");
    }

    // Leer datos del archivo
    read(in_fd, &numeros, NUM_ELEMENTOS * sizeof(int));

    // Calcular los rangos para cada hilo
    int mitad = NUM_ELEMENTOS / 2;
    strucAg[0].inicio = 0;
    strucAg[0].fin = mitad;
    strucAg[1].inicio = mitad;
    strucAg[1].fin = NUM_ELEMENTOS;
    //
       printf("TODO:\n");
    for (int i = 0; i < NUM_ELEMENTOS; i++)
    {
         printf("(%d - %d) ", i,numeros[i]);
    }
   printf("\n");
    // Crear hilos para ordenar
    pthread_create(&hilo1, NULL, intermedio, (void *)&strucAg[0]);
    pthread_create(&hilo2, NULL, intermedio, (void *)&strucAg[1]);

    // Esperp a que los hilos terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    // Imprimir resultados de cada hilo
    printf("RESULTADO HILO 1:\n");
    for (int i = 0; i < mitad; i++)
    {
        printf("(%d - %d)", i,numeros[i]);
    }
    printf("\n");
    printf("RESULTADO HILO 2:\n");
    for (int i = mitad; i < NUM_ELEMENTOS; i++)
    {
         printf("(%d - %d) ", i,numeros[i]);
    }
    printf("\n");
  
   
  
    close(in_fd);
    return 0;
}
//el cero esta la pos 840 y pos 992