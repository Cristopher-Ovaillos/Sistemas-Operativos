#ifndef _MI_TIPO_H_
#define _MI_TIPO_H_

// Definir la estructura del nuevo tipo de variable
struct miEstructura {
    int dato1;
    char dato2;
    // Puedes agregar más campos según sea necesario
};

// Declarar una función para inicializar una variable del nuevo tipo
void inicializarMiEstructura(struct miEstructura *variable, int valor1, char valor2);

#endif /* _MI_TIPO_H_ */