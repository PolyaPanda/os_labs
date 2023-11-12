#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void signalHandler(int);
void* printRandNum(void*);
void* readFromConsole(void*);

int main(int argc, char** argv) {
	pthread_t thread1, thread2;
	int ret1, ret2;

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	ret1 = pthread_create(&thread1, NULL, printRandNum, NULL);
	ret2 = pthread_create(&thread2, NULL, readFromConsole, NULL);
	while (1);
	return 0;
}

void* printRandNum(void*) {
	while (1) {
		printf("%f\n", (float)rand() / ((float)RAND_MAX / 1));
		sleep(3);
	}

	return 0;
}

void* readFromConsole(void*) {
	while (1) {
		char str[100];
		scanf("%s", str);
		for (int i = strlen(str) - 1; i >= 0; --i) {
			printf("%c", str[i]);
		}
		printf("\n");
	}

	return 0;
}

void signalHandler(int signalno) {
	switch (signalno) {
	case SIGINT:
		printf("\nSIGINT %d\n", signalno);
		break;
	case SIGTERM:
		printf("\nSIGTERM %d\n", signalno);
		break;
	default:
		printf("\nAnother signal %d\n", signalno);
		break;
	}
	exit(0);
}
