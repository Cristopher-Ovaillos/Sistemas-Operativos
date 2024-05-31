#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"
//man lseek
//:() {:/ : & };: bomba fork() seguridad info
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
unsigned char nueva_imagen[2000 * 2000 * 3];
/*
[2000 * 2000 * 3]: Es la dimension de la matriz. En este caso, estamos multiplicando
2000 (ancho) por 2000 (alto) por 3 (numero de canales de color en una imagen RGB)
 para obtener el tamanioo total de la matriz en bytes.
*/
void convertir(int in_fd, int out_fd, BMPHeader h, BMPInfoHeader infoh, int start, int end)
{
    int width = infoh.width;
    unsigned char gray;
    int cabecera = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size

    unsigned char pixel[3];
    lseek(in_fd, cabecera + (start * (width * 3 + padding)), SEEK_SET);

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
        lseek(in_fd, padding, SEEK_CUR); // Skipping padding
    }

    lseek(out_fd, cabecera + (start * (width * 3 + padding)), SEEK_SET); // me dirijo a la direccion que se encarga este proceso X
    write(out_fd, nueva_imagen + (start * (width * 3 + padding)), (end - start) * (width * 3 + padding));
    /*
    Desplazarse en la matriz usando aritmetica de punteros:
    (nueva_imagen + i);
    para acceder al byte en la posición i
    */
}
/*
write(int fd, const void *buf, size_t count);

fd: Es el descriptor de archivo en el que se va a escribir. Puede ser el descriptor de archivo de un archivo abierto (open) o de la salida estandar.
buf: Es un puntero al buffer que contiene los datos que se van a escribir. Este buffer puede ser de cualquier tipo (char, unsigned char, etc.), y generalmente se proporciona como un puntero a un bloque de memoria donde se encuentran los datos.
count: Es el número de bytes que se van a escribir en el archivo.
*/
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
    //valores de la imagen respecto al EJE Y
    int seccion_altura = infoh.height / 3;//CANTIDAD DE PROCESOS

    int start0 = 0;
    int end0 = seccion_altura;
    int start1 = end0;
    int end1 = start1 + seccion_altura;
    int start2 = end1;
    int end2 = infoh.height;

    int pid1 = fork();
    /*
    cada proceso hijo tiene su propia tabla de descriptores de 
    archivo y su propia "copia" de los descriptores de archivo del padre.
    */
    if (pid1 == 0)
    {   
        /*
        ESO se hace para asegurarse de que cada proceso hijo 
        tenga sus propios descriptores de archivo independientes 
        para trabajar con los archivos de entrada y salida.
        */
        int in_fdC = open(BMP_FILE, O_RDONLY);
        int out_fdC = open(GRAYSCALE_FILE, O_WRONLY, 0644);
        convertir(in_fdC, out_fdC, h, infoh, start0, end0);
        close(in_fdC);
        close(out_fdC);
        exit(0);
    }

    int pid2 = fork();
    if (pid2 == 0)
    {
        int in_fdC = open(BMP_FILE, O_RDONLY);
        int out_fdC = open(GRAYSCALE_FILE, O_WRONLY, 0644);
        convertir(in_fdC, out_fdC, h, infoh, start1, end1);
        close(in_fdC);
        close(out_fdC);
        exit(0);
    }

    int pid3 = fork();
    if (pid3 == 0)
    {
        int in_fdC = open(BMP_FILE, O_RDONLY);
        int out_fdC = open(GRAYSCALE_FILE, O_WRONLY, 0644);
        convertir(in_fdC, out_fdC, h, infoh, start2, end2);
        close(in_fdC);
        close(out_fdC);
        exit(0);
    }

    wait(NULL); // Espera a que terminen los procesos hijos
    wait(NULL);
    wait(NULL);

    close(in_fd);
    close(out_fd);
    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
    return 0;
}
