#include <xinu.h>
void produce(void),consume(void);
int32 n=0;

void Ejercicio2(void)   {


    int pid1,pidb;
int pidA=create(consume,4096,20,"cons",0);
int pidB=create(produce,4096,20,"prod",0);
resume(pidA);
resume(pidB);
recvclr();
receive();
} 
void produce(void){
    int32 i;
    for ( i = 0; i < 2000; i++)
    {
    n++;
    }
    
} 

void consume(void){
    int32 i;
    for ( i = 0; i < 2000; i++)
    {
    printf("The value of n is %d \n ",n);
    }
    
} 
