#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
// LEER ANOTACIONES
#define PGM_FILE "cat.pgm"
#define INVERT_FILE "catInvertido.pgm"
// arreglo global

#define ENCABEZADO 15

void something_wrong(int fd, const char *m)
{
    if (fd >= 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main()
{

    int in_fd = open(PGM_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong(in_fd, "Error open");
    }
    int out_fd = open(INVERT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
        something_wrong(out_fd, "Error en output");
    }

    //
  
    off_t tamanioBytes = lseek(in_fd, 0, SEEK_END);
      unsigned char imagen[1024*1024];
    write(in_fd, &imagen, 15);

      unsigned char pixel;
    for (off_t i = tamanioBytes ; 15 < i; i--){
        read(in_fd, pixel, 1);
        imagen[i] = pixel;
    }

    write(out_fd,imagen,tamanioBytes);

  

    printf("%d ", tamanioBytes - 15);

    close(in_fd);
    close(out_fd);
}