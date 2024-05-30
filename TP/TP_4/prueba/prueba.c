#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

unsigned char nueva_imagen[2000 * 2000 * 3];

void convertir(int in_fd, int start, int end, int width, int height, int padding, int out_fd) {
    unsigned char pixel[3];
    unsigned char gray;

    lseek(in_fd, start, SEEK_SET);
    lseek(out_fd, start, SEEK_SET);

    for (int y = start ; y < end ; y++) {
        for (int x = 0; x < width; x++) {
            read(in_fd, pixel, 3);
            gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
            nueva_imagen[y * (width * 3 + padding) + x * 3] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 1] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 2] = gray;
        }
        lseek(in_fd, padding, SEEK_CUR); // Skipping padding
    }

    lseek(out_fd, start, SEEK_SET);
    write(out_fd, &nueva_imagen[start], (end - start));
}

void something_wrong(int fd, const char *m)
{
    if (fd > 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main()
{
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong(in_fd, "Error al abrir el archivo de entrada");
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));

    if (h.magic[0] != 'B' || h.magic[1] != 'M')
    {
        something_wrong(in_fd, "El archivo no es un BMP válido");
    }

    BMPInfoHeader infoh;
    read(in_fd, &infoh, sizeof(BMPInfoHeader));

    if (infoh.bpp != 24 || infoh.compression != 0)
    {
        something_wrong(in_fd, "El BMP no es de 24 bits o está comprimido");
    }

    int width = infoh.width;
    int height = infoh.height;
    int padding = (4 - (width * 3) % 4) % 4;

    int start1 = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    int end1 = start1 + height * (width * 3 + padding);

    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));

    pid_t pid1, pid2, pid3;

    if ((pid1 = fork()) == 0)
    {
        convertir(in_fd, start1, end1, width, height, padding, out_fd);
        exit(0);
    }
    else if (pid1 < 0)
    {
        something_wrong(in_fd, "Error en fork 1");
    }

    if ((pid2 = fork()) == 0)
    {
        convertir(in_fd, start1, end1, width, height, padding, out_fd);
        exit(0);
    }
    else if (pid2 < 0)
    {
        something_wrong(in_fd, "Error en fork 2");
    }

    if ((pid3 = fork()) == 0)
    {
        convertir(in_fd, start1, end1, width, height, padding, out_fd);
        exit(0);
    }
    else if (pid3 < 0)
    {
        something_wrong(in_fd, "Error en fork 3");
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    close(in_fd);
    close(out_fd);
    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
    exit(0);
}
