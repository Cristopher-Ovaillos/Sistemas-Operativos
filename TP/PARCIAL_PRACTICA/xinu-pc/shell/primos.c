 /* primos entre el nÃºmero 1 y MAX */

 #include <xinu.h>

 #define MAX 500000

 int total = 0;

 int encontrar_primos(int from, int to)
 {
         int i, n, primo;
         printf( "\n   " );

          for ( i = from ; i <= to ; i++ ) {
                  primo = 1;
                  n = 2;

                   while ( n <= i / 2 && primo ) {
                           if ( i % n == 0 )
                                   primo = 0;

                            n++;
                   }

                   if ( primo )
                           printf( "%d ", i );
          }

          total++;
          return 0;
 }
/*

b. Crear un proceso control que pone a ejecutar dos procesos de encuentra primos. El primer proceso
debe encontrar los primos del 0 a 5000. El segundo proceso debe encontrar los primos del 5001 al
9999.
Reutilice código. Los 2 procesos encuentra primos deben utilizar el mismo código (esto significa que
deberá crear los procesos pasando parámetros, para que cada proceso sepa entre qué márgenes de
números buscar primos).


*/
void control_p() {
    int pidA,pidB;

    pidA=create(encontrar_primos, 1024, 20, "Proceso A", 2, 0, 5000);
    pidB=create(encontrar_primos, 1024, 20, "Proceso B", 2, 5001, 9999);
} 


 int primos()
 {

         encontrar_primos(1, MAX);
         //printf("Total : %i \n", total);
    //control();
          return 0;
 }

