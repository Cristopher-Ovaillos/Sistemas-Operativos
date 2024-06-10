#include <xinu.h>
#include "shprototypes.h"

void ahorcadoUnix(void)
{
  
    char palabraSecreta[] = "ejemplo";
    int tamanio = 0, intentos = 6, x = 0, i = 0;
    ;
    // Calcular el tamanioo de la palabraSecreta
   tamanio = strlen(palabraSecreta);
    
    char palabra[tamanio + 1]; // Tamanio igual al de palabraSecreta mas el nulo
    while (i < tamanio)
    {
        palabra[i] = '_';
        i++;
    }
    palabra[tamanio] = '\0'; // se agrega el caracter nulo
    int c;
    int total;
    total=0;
      // system ("/bin/stty raw");
    control(CONSOLE, TC_MODER, 0, 0);
    while (1)
    {
        printf("\r                                                          ");
        printf("\rPalabra: %s   Fallos: %d/%d c = %c  ingrese una letra (0 para salir): ", palabra, x, intentos, c);
        c = getchar();

        if (x >= intentos || c == '0')
        {
            break;
        }
        else
        {
            int acierto = 0; // Variable para verificar si se ha acertado una letra y se reinicia en cada iteracion
            for (i = 0; i < tamanio; i++)
            {
                if (palabra[i] == '_' && palabraSecreta[i] == c)
                {
                    palabra[i] = c;
                    acierto = acierto + 1; // Indica que se ha acertado una letra, el cero aca representa el falso y cualquier otro num el true
                }
            }
            if (!acierto)
            { // Si no se ha acertado ninguna letra, incrementofallos
                x++;
            }
            total=acierto+total;
        
        }
        
        if (total == tamanio)
		{
			printf("\n Ganaste era : %s\n ",palabraSecreta);
			break;
		}
		if (x == intentos)
		{
			printf("\n Perdiste\n");
			break;
		}

        
    }

    control(CONSOLE, TC_MODEC, 0, 0);
    // system ("/bin/stty sane erase ^H");
   
}