/* kbdread.c  -  kbdread */


#include <xinu.h>
#include <mouse.h>

devcall	kbdread (
	  struct dentry	*devptr,	
	  char          *buffer,     
          uint32        count         
	)
{
	//AGREADO
	buffer[0] = kbdgetc();
	//
	return OK;
}


