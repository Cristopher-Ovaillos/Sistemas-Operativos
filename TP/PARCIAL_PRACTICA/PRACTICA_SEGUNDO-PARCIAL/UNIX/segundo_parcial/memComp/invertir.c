#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PGM_FILE "cat.pgm"
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
    // Crear un objeto de memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    // Ajustar el tamaño del segmento de memoria compartida
    ftruncate(shm_fd, tamanioBytes);

    // Mapear el segmento de memoria compartida
    unsigned char *imagen = mmap(0, tamanioBytes, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (imagen == MAP_FAILED)
    {
        close(in_fd);
        close(shm_fd);
        something_wrong("Error al mapear el segmento de memoria compartida");
    }
    // Volver al inicio del archivo de entrada
    lseek(in_fd, 0, SEEK_SET);
    // Leer el encabezado del archivo
    read(in_fd, imagen, ENCABEZADO);

    unsigned char pix;
   for (off_t i = tamanioBytes - 1; i >= ENCABEZADO; i--)
    {
        read(in_fd, &pix, 1);

        imagen[i] = pix;
    }
    close(in_fd);
    return 0;
}
