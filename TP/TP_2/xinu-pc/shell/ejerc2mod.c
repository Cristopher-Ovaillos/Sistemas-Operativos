#include <xinu.h>

sid32 semProd, semCons;

void produceMOD(void);
void consumeMOD(void);
int32 m = 0;



void rod(void) {
    int32 i;
    for (i = 0; i < 2000; i++) {
        wait(semProd);
        m++;
        signal(semCons);
    }
    
}

void ons(void) {
    int32 i;
    for (i = 0; i < 2000; i++) {
        wait(semCons);
        printf("The value of n is %d\n", m);
        signal(semProd);
    }
}

void ejerc2mod(void) {
    semProd = semcreate(1);
    semCons = semcreate(0);

    resume(create(ons, 4096, 20, "cons", 0));
    resume(create(rod, 4096, 20, "prod", 0));
    
    recvclr();
    receive();
}