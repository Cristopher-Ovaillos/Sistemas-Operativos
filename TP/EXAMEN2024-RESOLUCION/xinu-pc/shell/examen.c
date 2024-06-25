#include <xinu.h>

int pidConsola, pidPrimo, pidBisiesto;

void esPrimo()
{
    int numero = receive();
    const char *valPrimo = "primo";
    int salirBucle = 1;
    int i = 2;

    if (numero <= 1)
    {
        valPrimo = "no primo";
    }
    else
    {
        while (salirBucle && (i * i) <= numero)
        {
            if ((numero % i) == 0)
            {
                valPrimo = "no primo";
                salirBucle = 0;
            }
            i++;
        }
    }
    send(pidConsola, valPrimo);
    while (1)
    {
        
    }
}

void bisiesto()
{
    int i = receive();
    const char *retornar = "no bisiesto";

    if ((i % 4) == 0)
    {
        if ((i % 100) != 0 || (i % 400) == 0)
        {
            retornar = "bisiesto";
        }
    }
    send(pidConsola, retornar);
    while (1)
    {
        
    }
    
}

void examen(void)
{
    char buf[10];
    int i;
    const char *txt1;
    const char *txt2;

    pidConsola = getpid();
    pidPrimo = create(esPrimo, 4096, 20, "primo", 0);
    pidBisiesto = create(bisiesto, 4096, 20, "bisiesto", 0);
    
    resume(pidBisiesto);
    resume(pidPrimo);

    printf("INGRESE UN NRO: ");
    read(CONSOLE, buf, 10);
    i = atoi(buf);

    send(pidBisiesto, i);
    txt1 = recvclr();
    printf("es %s \n", txt1);

    txt2 = recvclr();
    send(pidPrimo, i);
    printf("es %s \n", txt2);

    kill(pidPrimo);
    kill(pidBisiesto);
}

