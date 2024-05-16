#include <xinu.h>

extern void galaga(void); //  especificar el tipo de retorno void
extern void vidaypuntaje(void);
pid32 pidVida;
pid32 pidGalaga;
pid32 pidControl;
shellcmd procesoA(void) {
  pidGalaga = create(galaga, 1024, 20, "galaga", 0); // 
  pidVida = create(vidaypuntaje, 1024, 20, "vida", 0); // 
  pidControl=currpid;
    resume(pidGalaga);
    resume(pidVida);
    receive(); 
    printf("PROCESO 3 MATA");
    kill(pidGalaga);
    kill(pidVida);
    return OK; // 
}
