#include <xinu.h>
unsigned char tecla_actual;
unsigned char teclas[3];
//valor de teclas cuando aprieto
#define BUTTON_A	0x24
#define BUTTON_RIGHT	0x1f
#define BUTTON_LEFT	0x1e
//valor de teclas cuando suelto
#define BUTTON_A_REL	0xa4 
#define BUTTON_RIGHT_REL	0x9f
#define BUTTON_LEFT_REL	0x9e
extern sid32 semCerro;

int32 procTeclado (void) {
   
    teclas[0] = 0;
    teclas[1] = 0;
    teclas[2] = 0;
    
    open(KEYBOARD,0,0);//solo el galaga lo puede hacer
    while (1)
    {
        //tecla_actual = getc(KEYBOARD);
       read(KEYBOARD,&tecla_actual,1);
        //cuando suelto la tecla de las tengo que reiniciar a cero porque si no, el juego se movera infinatemente hacia la donde estaba previamente
        switch (tecla_actual)
        {
        case BUTTON_A : teclas[0] = 1; break;
        case BUTTON_LEFT : teclas[1] = 1; break;
        case BUTTON_RIGHT : teclas[2] = 1; break;
        case BUTTON_A_REL : teclas[0] = 0; break;
        case BUTTON_LEFT_REL : teclas[1] = 0; break;
        case BUTTON_RIGHT_REL : teclas[2] = 0; break;
        }
        
    }
    close(KEYBOARD); //solo el galaga lo puede hacer
  
    signal(semCerro);
   
}