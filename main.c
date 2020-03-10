//
// Created by lothar on 10/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int end = 0;

void truc(int sig){
    printf("On reçoit %i \n", sig);
    end = 1;
}

int main(void){
    signal(2, truc); //kill -2 id == ctrl C
    signal(15, truc);
    while (end == 0);
    return EXIT_SUCCESS;
}
//int signal (int signo, void * func);