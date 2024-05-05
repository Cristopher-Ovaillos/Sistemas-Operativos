#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h> /* Para ftruncate */
#include <fcntl.h>  /* Para constantes O_* */
#include <errno.h>  /* Para obtener información sobre errores */

const int SIZE = 4096;
const char *name = "OS";

int shm_fd;
void *ptr;
//          USAR
//gcc -o B B.c -lrt
int main()
{
    /* Abrir segmento de memoria compartida, solo lectura */
    shm_fd = shm_open(name, O_RDONLY, 0666);

    /* Mapear segmento de memoria compartida en el espacio de dirección del proceso */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Mapeo fallido: %d\n", errno);
        return -1;
    }

    /* Mostrar desde el segmento de memoria compartida */
    printf("Contenido de la memoria compartida:\n%s\n", (char *)ptr);

    /* Borramos el segmento de memoria compartida */
    if (shm_unlink(name) == -1)
    {
        printf("Error borrando %s\n", name);
        return -1;
    }

    return 0;
}
