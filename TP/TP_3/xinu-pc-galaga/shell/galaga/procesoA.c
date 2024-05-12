#include <xinu.h>

extern void galaga(void); //  especificar el tipo de retorno void
extern void vidaypuntaje(void);
pid32 pidVida;
pid32 pidGalaga;

shellcmd procesoA(void) {
  pidGalaga = create(galaga, 1024, 20, "galaga", 0); // 
  pidVida = create(vidaypuntaje, 1024, 20, "vida", 0); // 
    resume(pidGalaga);
    resume(pidVida);
    return OK; // 
}
