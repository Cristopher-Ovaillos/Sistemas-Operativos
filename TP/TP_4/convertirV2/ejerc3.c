/* Ejemplo con threads en Linux. Compilar con: gcc -o threads convertir3Thread.c -lpthread -lrt */
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
    for (int y = start; y < end; y++)
    {
        for (int x = 0; x < width; x++)
        {
            read(in_fd, pixel, 3);
            gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
            nueva_imagen[y * (width * 3 + padding) + x * 3] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 1] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 2] = gray;
          
        }
        
    }
    
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
 int padding = (4 - (infoh.width* 3) % 4) % 4;
    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    read(in_fd, &nueva_imagen,infoh.width * infoh.height * 3+padding);
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
        args[i].in_fd = in_fd;
        args[i].out_fd = out_fd;
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

    write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);

    close(in_fd);
    close(out_fd);

    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
    return 0;
}
