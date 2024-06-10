/* kbdclose.c  -  kbdclose */

#include <xinu.h>
//incluir keyboard
#include <keyboard.h>
/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */

//estos es estan definidos desde keyboard
extern sid32 semKbd;
extern pid32 pidKbd;

devcall	kbdclose (
	  struct dentry	*devptr		// Entry in device switch table	
	)
{
		if (pidKbd == getpid())
	{
		signal(semKbd);
	}
}


