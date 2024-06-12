#include <xinu.h>
//llamar a los archivos.c
extern void galaga(void); 
extern void vidaypuntaje(void);
extern void procTeclado(void);
pid32 pidVida;
pid32 pidGalaga;
pid32 pidControl;
//nuevo proceso
pid32 pidTeclado;
sid32 semCerro;
int vidas;
shellcmd controlG(void) {
  pidGalaga = create(galaga, 1024, 20, "galaga", 0); // 
  pidVida = create(vidaypuntaje, 1024, 20, "vida", 0); // 
  //nuevo proceso
  pidTeclado=create(procTeclado,1024,20,"procTeclado",0);
  pidControl=currpid; //process.h currpid devuelve pid.
  printf("LOS PROCESOS INICIAN");
    resume(pidGalaga);
    resume(pidVida);
    resume(pidTeclado);
    receive(); 
    kill(pidGalaga);
    kill(pidVida);
     wait(semCerro);
    kill(pidTeclado);
    kill(pidControl);
   
    return OK; // 
}
