
/*
#include <stdio.h>
#include <stdlib.h>
system ("/bin/stty raw");
system ("/bin/stty sane erase ^H");

#include <stdio.h>: Esta línea incluye la librería estándar de entrada/salida en C, 
que proporciona funciones para realizar operaciones de entrada y salida, como printf y scanf.

#include <stdlib.h>: Esta línea incluye la librería estándar de funciones generales en 
C, que proporciona funciones para la gestión de memoria, generación de números aleatorios, 
conversión de cadenas, entre otras operaciones.

system("/bin/stty raw");: Esta línea ejecuta un comando del sistema operativo utilizando 
la función system. El comando /bin/stty raw configura el modo de entrada del terminal 
en "raw", lo que significa que las teclas presionadas se envían directamente al 
programa sin esperar por la tecla Enter. Esto es útil para aplicaciones interactivas 
como juegos.

system("/bin/stty sane erase ^H");: Esta línea tambien ejecuta un comando del 
sistema operativo utilizando la función system. 
El comando /bin/stty sane erase ^H restaura la configuración normal del 
terminal después de que el programa ha terminado, donde ^H representa la tecla 
retroceso (Backspace) que se usa para borrar caracteres en la línea de entrada.

*/

#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */

int main() {
 system ("/bin/stty raw"); 
    char palabraSecreta[] = "ejemplo";
    int tamanio = 0,intentos=6,x=0,i = 0;;
    // Calcular el tamanioo de la palabraSecreta
    while (palabraSecreta[tamanio] != '\0') {
        tamanio++;
    }
    char palabra[tamanio + 1];  // Tamanio igual al de palabraSecreta mas el nulo
    while (i < tamanio) {
        palabra[i] = '_';
        i++;
    }
    palabra[tamanio] = '\0';  // se agrega el caracter nulo 
    int c; 
    while(1) {
		printf("\r                                                          ");
        printf("\rPalabra: %s   Fallos: %d/%d c = %c  ingrese una letra (0 para salir): ",palabra, x,intentos ,c);
		c = getchar();

		
        if (x >= intentos || c == '0') {
            break;
        } else {
            int acierto = 0;  // Variable para verificar si se ha acertado una letra y se reinicia en cada iteracion
            for (i = 0; i < tamanio; i++) {
                if (palabra[i] == '_' && palabraSecreta[i] == c) {
                    palabra[i] = c;
                    acierto = 1;  // Indica que se ha acertado una letra, el cero aca representa el falso y cualquier otro num el true
                }
            }
            if (!acierto) {  // Si no se ha acertado ninguna letra, incrementofallos
                x++;
            }
        }
	}

	
	system("/bin/stty sane erase ^H");

    return 0;
}