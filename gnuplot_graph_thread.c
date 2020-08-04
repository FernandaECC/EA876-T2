#include <stdio.h>
#include <stdlib.h>
#define GNUPLOT "gnuplot -persist"
  
int main(int argc, char **argv)
{
    FILE *gp;
    gp = popen(GNUPLOT, "w");
    if (gp == NULL) {
        printf("Erro ao abrir pipe para o GNU plot.\n"
            "Instale com 'sudo apt-get install gnuplot'\n");
        exit(0);
    }
    fprintf(gp, "plot sin(0)\n");
    fprintf(gp, "rep 'plot_thread.txt' using 1:2 with lines, 'plot_linear.txt' using 1:3 with lines \n");
    fclose(gp);
}
