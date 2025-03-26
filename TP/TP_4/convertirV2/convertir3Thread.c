/* Ejemplo con threads en Linux. Compilar con: gcc -o threads convertir3Thread.c -lpthread -lrt */

/*CRIS-2025 - CORREGIR, NO SE USA SEMAFOROS FUE MI ERROR USARLO ACA NO ERA NECESARIO
Por esta razon:
En este código se utilizan semáforos para proteger la escritura sobre el arreglo global nueva_imagen y la escritura en el archivo de salida. Sin embargo, dado 
que cada hilo procesa una sección diferente de la imagen (sin solapamientos) y cada hilo abre sus propios(importante) descriptores de archivo (por lo tanto, tienen posiciones 
independientes al usar lseek), no existe realmente una condición de carrera en la escritura de datos.

ademas, aca lo use el semaforo para que bueno mi programa principal esperara a que terminaran estos hilos.
pero existe join. El pthread_join se utiliza para esperar a que un hilo específico termine su ejecución.


version mejorada: https://github.com/guillermo-diaz/sistemas-operativos/blob/master/TP4/ex03fixed.c
en esta version en vez de cada hilo escribiera lo que hicimos fue hacer un arreglo de toda la imagen (asignando incio y fin para cada hilo), Y LUEGO este programa no los hilos
es el encargado de escribirlo y crear una salida.
*/
/* Ejecutar con ./threads */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

#pragma pack(push, 1)
typedef struct
{
    unsigned char magic[2];
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct
{
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bpp;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerM;
    int yPixelsPerM;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

typedef struct
{
    int in_fd;
    int out_fd;
    BMPHeader h;
    BMPInfoHeader infoh;
    int start;
    int end;
} arg_threads;

unsigned char nueva_imagen[2000 * 2000 * 3];
sem_t sem;

void *convertir(void *strucArg)
{
    arg_threads *args = (arg_threads *)strucArg;
    int in_fd = args->in_fd;
    int out_fd = args->out_fd;
    BMPHeader h = args->h;
    BMPInfoHeader infoh = args->infoh;
    int start = args->start;
    int end = args->end;

    int width = infoh.width;
    unsigned char gray;
    int cabecera = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    int padding = (4 - (width * 3) % 4) % 4;

    unsigned char pixel[3];
    lseek(in_fd, cabecera + (start * (width * 3 + padding)), SEEK_SET);

    for (int y = start; y < end; y++)
    {
        for (int x = 0; x < width; x++)
        {
            read(in_fd, pixel, 3);
            gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
            sem_wait(&sem);
            nueva_imagen[y * (width * 3 + padding) + x * 3] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 1] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 2] = gray;
            sem_post(&sem); 
        }
        lseek(in_fd, padding, SEEK_CUR);
    }
    sem_wait(&sem);
    lseek(out_fd, cabecera + (start * (width * 3 + padding)), SEEK_SET); // me dirijo a la direccion que se encarga este proceso X
    write(out_fd, nueva_imagen + (start * (width * 3 + padding)), (end - start) * (width * 3 + padding));
    sem_post(&sem); 
}

void something_wrong(int fd, const char *m)
{
    if (fd >= 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main()
{
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong(in_fd, "Error open");
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));

    if (h.magic[0] != 'B' || h.magic[1] != 'M')
    {
        something_wrong(in_fd, "No es BMP");
    }

    BMPInfoHeader infoh;
    read(in_fd, &infoh, sizeof(BMPInfoHeader));

    if (infoh.bpp != 24 || infoh.compression != 0)
    {
        something_wrong(in_fd, "No 24-bit BMP");
    }

    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
        something_wrong(in_fd, "Error en output");
    }

    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));

    int seccion_altura = infoh.height / 3;

    int start0 = 0;
    int end0 = seccion_altura;
    int start1 = end0;
    int end1 = start1 + seccion_altura;
    int start2 = end1;
    int end2 = infoh.height;

    pthread_t tid[3];
    arg_threads args[3];

    for (int i = 0; i < 3; i++)
    {
        //SI O SI DEBO HACER ESTO, CADA HILO DEBE TENER SU PROPIO CODIFICACION DE LA IMAGEN
        args[i].in_fd = open(BMP_FILE, O_RDONLY);
        args[i].out_fd = open(GRAYSCALE_FILE, O_WRONLY, 0644);
        args[i].h = h;
        args[i].infoh = infoh;
    }

    args[0].start = start0;
    args[0].end = end0;

    args[1].start = start1;
    args[1].end = end1;

    args[2].start = start2;
    args[2].end = end2;

    sem_init(&sem, 0, 1); 

    pthread_create(&tid[0], NULL, convertir, (void *)&args[0]);
    pthread_create(&tid[1], NULL, convertir, (void *)&args[1]);
    pthread_create(&tid[2], NULL, convertir, (void *)&args[2]);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    sem_destroy(&sem); //PARA ELIMINAR EL SEMAFORO QUE SE CREOO
    //write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);

    close(in_fd);
    close(out_fd);

    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
    return 0;
}
