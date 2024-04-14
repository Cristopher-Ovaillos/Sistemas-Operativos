#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */

int main() {
 system ("/bin/stty raw"); 
    char palabraSecreta[] = "abcdefgh";
    int tamanio = 0,intentos=6,x=0,i = 0;;
    // Calcular el tamaño de la palabraSecreta
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