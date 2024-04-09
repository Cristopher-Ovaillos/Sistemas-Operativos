/* Creación y finalización de proceso en Linux */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void ejercicio7bis(void) {
    int pid;
    int x = 0;
    pid = fork();
    if (pid == 0) {
        execl("/bin/date", "date", NULL);
    } else {
        sleep(5);
        kill(pid, SIGKILL);
        printf("Maté a mi propio hijo (suena horrible)\n");
    }
}

int main() {
    ejercicio7bis();
    return 0;
}
