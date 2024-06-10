/*  main.c  - main */
#include <xinu.h>

extern paint_screen();
extern print_text_on_vga(unsigned int x, unsigned int y, const char * t);

process	main(void)
{
    int pidA, pidB;

    paint_screen();
    print_text_on_vga(10, 200, "Xinu OS for PC with VGA support");
    print_text_on_vga(10, 220, "Sistemas Operativos I");
    print_text_on_vga(10, 240, "Cristopher Ovaillos");

    //mi_programa(&pidA, &pidB);

    //sleepms(10000);

    //kill(pidA);
    //kill(pidB);
    //sleepms(2000);
    recvclr();
    resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

    while (TRUE) {
        recvclr();
        receive();
        sleepms(200);
        // Recrear el proceso de shell
        resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
    }
    return OK;
}

