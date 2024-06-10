#include <xinu.h>
extern void galaga(void); 
extern void vidaypuntaje(void);
pid32 pidVida;
pid32 pidGalaga;
pid32 pidControl;
int vidas;
shellcmd controlG(void) {
  pidGalaga = create(galaga, 1024, 20, "galaga", 0); // 
  pidVida = create(vidaypuntaje, 1024, 20, "vida", 0); // 
  pidControl=currpid; //process.h currpid devuelve pid.
  printf("LOS PROCESOS INICIAN");
    resume(pidGalaga);
    resume(pidVida);
    int msg=receive(); 
    kill(pidGalaga);
    kill(pidVida);
    kill(pidControl);
   
    return OK; // 
}
