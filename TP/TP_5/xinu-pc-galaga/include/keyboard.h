
/*
son alias para las funciones inb y outb, que se utilizan para realizar 
operaciones de entrada/salida (I/O) en puertos especificos.
*/
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)
/*
Es el registro de datos del teclado. Se utiliza para leer los datos enviados
 por el teclado (como las pulsaciones de teclas) y tambien para escribir 
 datos hacia el teclado.
*/
#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
/*
Es el registro de comandos del teclado. Se utiliza para enviar comandos al 
controlador del teclado.
*/
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)
#define BUFFER_SIZE 10 //Size of keyboard buffer

extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 

struct StBuffer
{   
    //buffer 
    unsigned char buffer [BUFFER_SIZE];
    //indice para la posicion actual de escritura en el buffer
    int32 index;
    //indica para la posicion actual de lectura
    int32 finIndex;
    //sincronizacion del acceso al buffer
    sid32 semInBf;
   
};