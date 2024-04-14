#include <xinu.h>
void contar(char c);

void Ejercicio1(void){
    int32 pid_a , ms_a , pid_b , ms_b ,pid_c , ms_c;
    pid_a = create(contar,1024,20,"repetir A",1,'A');
    pid_b = create(contar,1024,20,"repetir B",1,'B');
    pid_c = create(contar,1024,20,"repetir C",1,'C');
    resume(pid_c);
    resume(pid_b);
    resume(pid_a);
    ms_a =  120;
    ms_b =  60;
    ms_c =  20;
    high_level_scheduler(pid_a , ms_a,pid_b , ms_b,pid_c , ms_c);
    
}

void contar(char c){
    
    for (int i = 0; i < 20000000; i++)
    {
        if(i % 100000 == 0){
            kprintf("%d : %c",i,c);
        }
    }
    
}