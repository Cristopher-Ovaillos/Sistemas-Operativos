// se encarga de ver el teclado el byte
#include <xinu.h>
#include <keyboard.h>

extern struct StBuffer stbuffer;
extern pid32 pidKbd;

unsigned char kbdgetc()
{

 
   
    if (pidKbd == getpid())
    {
         unsigned char c = SYSERR;
        wait(stbuffer.semInBf);
        // wait(stbuffer.semBin);
        c = stbuffer.buffer[stbuffer.index];
        stbuffer.buffer[stbuffer.index] = 0;
        stbuffer.index = (stbuffer.index + 1) % BUFFER_SIZE;
        // signal(stbuffer.semBin);
        return c;
    }else{
        //system error
        return SYSERR;
    } 
    
}
