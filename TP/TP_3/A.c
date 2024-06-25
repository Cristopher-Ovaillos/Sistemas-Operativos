#include <sys/mman.h>
#include  <sys/stat.h>
#include <fcntl.h>  /* Para constantes O_* */
#include <unistd.h> /* Para ftruncate */
#include <stdio.h>



/*
shm_open() crea y abre un nuevo objeto de memoria compartida POSIX, o
abre uno existente. Un objeto de memoria compartida POSIX es, en efecto,
un identificador que puede ser utilizado por procesos no relacionados
para mapear la misma región de memoria compartida. La función shm_unlink()
realiza la operación opuesta, eliminando un objeto previamente creado por
shm_open().

 */
int shm_fd;
void *ptr;
// COMO USAR
//gcc -o A A.c -lrt
//cat /usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt           PARA VER EL ARCHIVO
// ls /dev/shm              PARA VER SI SE CREO 
int main()
{
    const int SIZE = 4096;
    const char *name = "OS";
    const char *pathName = "/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";
    int shm_fd,fileDescriptor;
    void *ptr;
     int buffSize = 1024;
    char buff[buffSize];
    //crear un objeto de memoria compartida POSIX.
     shm_fd= shm_open(name, O_CREAT | O_RDWR, 0666);
    /*
    _name es el nombre del objeto de memoria compartida. Este nombre es unico en 
    el msistema y se utiliza para identificar el objeto de memoria compartida
    _"O_CREAT | O_RDWR" Son las banderas de apertura que se pasan a shm_open.
    _"O_CREAT" Indica que se debe crear el objeto si no existe.
    _"O_RDWR" Indica que el objeto se abrirá para lectura y escritura."
    _"0666" Es el modo o permisos que se aplicarán al objeto de memoria compartida, en 
    este caso, permite lectura y escritura para todos los usuarios.
    shm_open,ftruncate,mmap
    */
   ftruncate(shm_fd, SIZE);
    /* ahora asigna el segmento de memoria compartida en el
 * espacio de direcciones del proceso */
 ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,shm_fd,0);
/*
0: Especifica que la direccion de inicio de la asignacion se elige 
automáticamente por el sistema operativo.

SIZE: Es el tamaño de la región de memoria que se va a asignar.

PROT_READ | PROT_WRITE: Indica que la región de memoria asignada se puede leer y escribir.

MAP_SHARED: Indica que la memoria asignada es compartida entre múltiples procesos y 
cualquier cambio realizado en esta memoria será visible para todos los procesos que compartan esta memoria.

shm_fd: Es el descriptor de archivo de la memoria compartida que se está asignando.

0: Es el desplazamiento dentro del archivo de memoria compartida, en este caso, se
 coloca al principio del archivo.

*/

 if (ptr == MAP_FAILED)
    {
        printf("Mapeo fallido\n");
        return -1;
    }
     /* Abrimos el archivo solo para lectura */
    fileDescriptor = open(pathName, O_RDONLY);
    /* Copiamos el contenido del txt en buff */
    read(fileDescriptor, buff, buffSize);
 /* Copiamos el contenido de buff en el segmento de memoria compartida */
    sprintf(ptr, "%s", buff);

    /* Cerramos el archivo */
    if (close(fileDescriptor) == -1)
    {
        printf("Error al cerrar el archivo");
    }


    return 0;
}