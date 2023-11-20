#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void sigHandler(int signalno) {
    switch (signalno) {
    case SIGINT:
        printf("\nSIGINT %d\n", signalno);
        printf("terminated by ctrl+c");
        break;
    case SIGTERM:
        printf("\nSIGTERM %d\n", signalno);
        printf("exited by terminated signal");
        break;
    default:
        printf("\nAnother signal\n");
        break;
    }
    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    while (1);
}
