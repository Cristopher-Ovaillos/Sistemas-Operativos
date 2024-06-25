#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>  /* Para obtener información sobre errores */

#define INVERT_FILE "catMemComp.pgm"
#define SHM_NAME "pgm_invert"

void something_wrong(const char *m)
{
    perror(m);
    exit(1);
}

int main()
{
    // Abrir el segmento de memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    // Obtener el tamaño del segmento de memoria compartida
    struct stat shm_stat;
    fstat(shm_fd, &shm_stat);
    off_t tamanioBytes = shm_stat.st_size;
    // Mapear el segmento de memoria compartida
    unsigned char *imagen = mmap(0, tamanioBytes, PROT_READ, MAP_SHARED, shm_fd, 0);
     if (imagen == MAP_FAILED)
    {
        printf("Mapeo fallido: %d\n", errno);
        return -1;
    }
    // Crear el archivo de salida
    int out_fd = open(INVERT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Escribir el contenido invertido en el archivo de salida
    write(out_fd, imagen, tamanioBytes);
    close(out_fd);
    // Desvincular y eliminar el segmento de memoria compartida
    if (munmap(imagen, tamanioBytes) == -1)
    {
        something_wrong("Error al liberar el segmento de memoria compartida");
    }

    if (shm_unlink(SHM_NAME) == -1)
    {
        something_wrong("Error al eliminar el segmento de memoria compartida");
    }

    return 0;
}
