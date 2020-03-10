#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int end = 0;

void truc(int sig){
    printf("On reçoit %i \n", sig);
    end = 1;
}

int main(void){
    signal(2, truc);
    while (end == 0);
    return EXIT_SUCCESS;
}