/* kbdopen.c  -  kbdopen */

#include <xinu.h>
#include <keyboard.h>
	extern sid32 semKbd;
	extern pid32 pidKbd;
devcall	kbdopen (
	 struct	dentry	*devptr,	
	 char	*name,		
	 char	*mode			
	)
{

	wait(semKbd); //semaforo de teclado
	pidKbd = getpid();//actualizo
}
