#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_sigint(int signum) {
    printf("\nReceived Ctrl+C, exiting...\n");
    exit(0);
}

int main() {
    signal(2, handle_sigint);  // Use the handler instead of SIG_IGN
	printf("SIGABRT value: %d\n", SIGABRT);
	printf("SIGFPE value: %d\n", SIGFPE);
	printf("SIGILL value: %d\n", SIGILL);
	printf("SIGINT value: %d\n", SIGINT);
	printf("SIGSEGV value: %d\n", SIGSEGV);
	printf("SIGTERM value: %d\n", SIGTERM);
    while (1) {
        printf("Press Ctrl+C to exit...\n");
        sleep(1);
    }
    return 0;
}