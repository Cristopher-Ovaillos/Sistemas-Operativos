//se encarga de ver el teclado el byte
#include <xinu.h>
#include <keyboard.h>

extern struct StBuffer stbuffer;
extern pid32 pidKbd;

unsigned char kbdgetc(){
    
    /*
     SYSERR ESTA DEFINIO EN INCLUDE/KERNEL.H
    inicializa la variable c con un valor de error 
    (SYSERR). Esta inicialización garantiza que si
     por alguna razón el proceso actual no 
     tiene acceso al teclado 
     (es decir, pidKbd != getpid()), la función 
     kbdgetc() devolverá un valor indicativo de 
     error. Es una practica de programación 
     defensiva que ayuda a evitar comportamientos 
     indefinidos o errores sutiles en el codigo.
    */
    unsigned char c = SYSERR;
    if(pidKbd == getpid()){
    c = receive();
    }
    return c;
    
   
    
}

