/* kbdclose.c  -  kbdclose */

#include <xinu.h>
//incluir keyboard
#include <keyboard.h>
/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */

//estos es estan definidos desde keyboard e inicializados en KBDINIT
extern sid32 semKbd;
extern pid32 pidKbd;

devcall	kbdclose (
	  struct dentry	*devptr		// Entry in device switch table	
	)
{
		if (pidKbd == getpid())
	{	
		//bueno el kbdclose si el proceso tiene al teclado entonces libera el permiso para que otro proceso lo pueda ocupar
		//por solo un proceso puede usar teclado no dos 
		signal(semKbd);
	}
}


