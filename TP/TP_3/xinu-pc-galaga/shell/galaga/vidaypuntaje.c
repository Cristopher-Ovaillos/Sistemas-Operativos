#include <xinu.h>
int32 vidas;
int32 puntaje;
int32 vidaypuntaje(void)
{
    vidas = 3;
    int32 msg;
    puntaje = 0;
    char cadena[30];
    while (1)
    {
        sprintf(cadena, "Vidas : %d Puntaje : %d", vidas, puntaje);
        print_text_on_vga(300, 40, cadena);
        msg = receive();
        switch (msg)
        {
        case -1:
            vidas--;
            break;
        case 0:
            vidas = 3;
            puntaje = 0;
            break;
        case 1:
            puntaje++;
            break;
        default:
            break;
        }
    }
}