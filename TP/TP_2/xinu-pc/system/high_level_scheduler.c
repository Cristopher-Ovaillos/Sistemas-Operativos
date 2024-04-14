#include <xinu.h>

void high_level_scheduler(int pid_a, int ms_a, int pid_b, int ms_b, int pid_c, int ms_c) {
    int prio_planificador = getprio(getpid()); 
    int prio_a, prio_b, prio_c; //declare esto para luego guardar la prioridad de los proc

    while (1) {
        prio_a = getprio(pid_a);
        prio_b = getprio(pid_b);
        prio_c = getprio(pid_c);

        chprio(pid_a, prio_planificador - 1);
        chprio(pid_b, prio_planificador - 1);
        chprio(pid_c, prio_planificador - 1);

        sleepms(ms_a); 
        chprio(pid_a, prio_a); //se utiliza para cambiar la prioridad de un proceso específico

        sleepms(ms_b); 
        chprio(pid_b, prio_b); 

        sleepms(ms_c); 
        chprio(pid_c, prio_c);
    }
}
