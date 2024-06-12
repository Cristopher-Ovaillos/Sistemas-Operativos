// se encarga de ver el teclado el byte
#include <xinu.h>
#include <keyboard.h>

/*
se encarga de leer un byte del buffer del teclado de manera
sincronizada, asegurando que solo el proceso con pidKbd pueda realizar
esta operacion.
*/
extern struct StBuffer stbuffer;
extern pid32 pidKbd;

unsigned char kbdgetc(){
    unsigned char c = SYSERR;
    if(pidKbd = getpid()){
    wait(stbuffer.semInBf);
    
    c = stbuffer.buffer[stbuffer.index];
    stbuffer.buffer[stbuffer.index] = 0;
    stbuffer.index = (stbuffer.index+1)%BUFFER_SIZE;
  
    }
    return c;
    
}