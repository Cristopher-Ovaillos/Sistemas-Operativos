#include <xinu.h>
#include <keyboard.h>

unsigned char kblayout[128];  // { ... } Fill your layout yourself 

struct StBuffer stbuffer;
sid32 semKbd;
pid32 pidKbd;

void keyboard_wait(byte a_type)
{
    int _time_out = 100000; 
    if(a_type == 0)
    {
        while(_time_out--) //Data
        {
            if((inportb(0x64) & 1) == 1)
            {
                return;
            }
        }
    }
    else
    {
        while(_time_out--) //Signal
        {
            if((inportb(0x64) & 2) == 0)
            {
                return;
            }
        }
    }
}

devcall kbdinit(struct dentry *devptr)
{
    semKbd = semcreate(1);
    pidKbd = -1; // sin asignacion
    stbuffer.semInBf = semcreate(0); // Inicialmente a 0 para que permita la primera operacion,se encarga el kbdhandler de liberar permisos
    stbuffer.index = 0;
    stbuffer.finIndex = 0;

    for (int i = 0; i < 128; i++) {
        kblayout[i] = i;
    }

    byte _status;

    // Enable the keyboard device
    keyboard_wait(1);
    outportb(0x64, 0xAE);

    // Enable the interrupts
    keyboard_wait(1);
    outportb(0x64, 0x20);
    keyboard_wait(0);
    _status = (inportb(0x60) | 1);
    keyboard_wait(1);
    outportb(0x64, 0x60);
    keyboard_wait(1);
    outportb(0x60, _status);

    // Clear any pending input
    while ((inportb(0x64) & 2) != 0) {
        inportb(0x60);
    }

    set_evec(1 + IRQBASE, (uint32)kbdhandlerirq);

    return OK;
}
