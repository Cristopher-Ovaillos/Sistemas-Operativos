#include <xinu.h>


// Definición de la estructura persona en Xinu
struct persona {
    char nombre[50];
    int edad;
    char genero;
};

// Función para imprimir los datos de una persona en Xinu
void imprimirPersona(struct persona *p) {
    printf("Nombre: %s\n", p->nombre);
    printf("Edad: %d\n", p->edad);
    printf("Genero: %c\n", p->genero);
}

void ejemploStruc(void) {
 
    struct persona alguien;
    strcpy(alguien.nombre, "Juan");
    alguien.edad = 30;
    alguien.genero = 'M';
    printf("           %s \n ",alguien.nombre);
    imprimirPersona(&alguien);

    
}
