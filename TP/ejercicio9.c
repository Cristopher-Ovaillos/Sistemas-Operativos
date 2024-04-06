
#include <stdio.h>

int main() {
    char variableChar = 127; // Valor maximo para char con signo 1 byte (8 bits) debido a que en ejercicios anteriores tipo el 8 donde pide el tamanio con sizeof
    unsigned char variableUnsignedChar = 255; // Valor maximo para unsigned char  1 byte(8 bits) no mas calcule el rango representable y es de 0-255
    //rango de char con signo es de -128,127
    printf("Valor de variableChar antes del incremento: %d\n", variableChar);
    printf("Valor de variableUnsignedChar antes del incremento: %u\n", variableUnsignedChar);
    //incremento
    variableChar++;
    variableUnsignedChar++;
    //pos incremento devuelve el inferior de cada uno
    printf("Valor de variableChar después del incremento: %d\n", variableChar);
    printf("Valor de variableUnsignedChar después del incremento: %u\n", variableUnsignedChar);

    return 0;
}