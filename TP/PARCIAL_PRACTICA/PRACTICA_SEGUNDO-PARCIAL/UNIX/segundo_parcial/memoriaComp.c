#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PGM_FILE "cat.pgm"
#define INVERT_FILE "catMemComp.pgm"
#define ENCABEZADO 15
#define SHM_NAME "pgm_invert"

void something_wrong(const char *m)
{
    perror(m);
    exit(1);
}

int main()
{
    int in_fd = open(PGM_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong("Error al abrir el archivo de entrada");
    }

    // Calculo el tamaño de la imagen
    off_t tamanioBytes = lseek(in_fd, 0, SEEK_END);
    if (tamanioBytes == -1)
    {
        close(in_fd);
        something_wrong("Error al obtener el tamaño del archivo de entrada");
    }

    // Crear un objeto de memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        close(in_fd);
        something_wrong("Error al crear el segmento de memoria compartida");
    }

    // Ajustar el tamaño del segmento de memoria compartida
    if (ftruncate(shm_fd, tamanioBytes) == -1)
    {
        close(in_fd);
        close(shm_fd);
        something_wrong("Error al ajustar el tamaño del segmento de memoria compartida");
    }

    // Mapear el segmento de memoria compartida
    unsigned char *imagen = mmap(0, tamanioBytes, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (imagen == MAP_FAILED)
    {
        close(in_fd);
        close(shm_fd);
        something_wrong("Error al mapear el segmento de memoria compartida");
    }

    // Volver al inicio del archivo de entrada
    if (lseek(in_fd, 0, SEEK_SET) == -1)
    {
        close(in_fd);
        close(shm_fd);
        something_wrong("Error al mover el puntero del archivo de entrada");
    }

    // Leer el encabezado del archivo
    if (read(in_fd, imagen, ENCABEZADO) != ENCABEZADO)
    {
        close(in_fd);
        close(shm_fd);
        something_wrong("Error al leer el encabezado del archivo de entrada");
    }

    unsigned char pix;
    for (off_t i = ENCABEZADO; i < tamanioBytes; i++)
    {
        if (read(in_fd, &pix, 1) != 1)
        {
            close(in_fd);
            close(shm_fd);
            something_wrong("Error al leer el archivo de entrada");
        }

        imagen[i] =  pix;
    }

    close(in_fd);
    return 0;
}
