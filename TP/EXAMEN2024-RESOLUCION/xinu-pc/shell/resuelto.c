#include <xinu.h>

#define NO_ES_PRIMO 0
#define ES_PRIMO 1
#define NO_ES_BISIESTO 2
#define ES_BISIESTO 3

int es_primo(int);

/* proceso que verifica si es primo */
void proceso2(int padre) {
	int n;

	n = receive();

	if (es_primo(n))
		send(padre, ES_PRIMO);
	else
		send(padre, NO_ES_PRIMO);

	/* 
	 * evitar por 10 seg que este proceso finalice, para que XINU 
	 * no envie un msg al padre de que proceso2 finalizó
	 */
	sleep(10);
}

/* proceso que verifica si es bisiesto */
void proceso3(int padre) {
	int anio;

	anio = receive();
	sleep(5); /* esperamos, para evitar que el padre pierda algun msg */

	if ((anio % 4 == 0) && ((anio % 100 != 0) || (anio % 400 == 0)))
		send(padre, ES_BISIESTO);	
	else
		send(padre, NO_ES_BISIESTO);
}

void mostrar_resultado(int msj) {

	if (msj==ES_BISIESTO) {
		printf ("el numero ingresado es bisiesto\n");
	} else if (msj==NO_ES_BISIESTO) {
		printf ("el numero ingresado NO es bisiesto\n");
	} else if (msj==ES_PRIMO) {
		printf ("el numero ingresado es primo\n");
	} else if (msj==NO_ES_PRIMO) {
		printf ("el numero ingresado NO es primo\n");
	}
}

void resuelto(){
	int n, pid, proc2, proc3, i;
	int msj;
	char buf[10];
    
	/* Solicitamos un nro al usuario */
	printf("INGRESE UN NRO: ");
	read(CONSOLE, buf, 10);
	n = atoi(buf);
	printf ("el numero ingresado entero es %d \n", n);
    
	pid = getpid();
    
	proc2 = create(proceso2, 8192, 20, "Proceso 1", 1, pid);
	proc3 = create(proceso3, 8192, 20, "Proceso 2", 1, pid);

	resume(proc2);
	resume(proc3);
	
	/* Enviamos el nro del usuario a los procesos */
	send(proc2, n);
	send(proc3, n);
	
	msj= receive();
	mostrar_resultado(msj);
	msj= receive();
	mostrar_resultado(msj);
}

int es_primo(int num) {
	int res, i;

	res = 1;	/* es primo */
	i = 2;
	while ((i<num) && (res)) {

		if ((num%i) == 0)
			res = 0;	/* no es primo */
		i++;
	}

	return res;
}