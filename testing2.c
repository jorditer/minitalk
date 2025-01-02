#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    fork();
    fork();
    fork();
    printf("hello\n");
    printf("%d\n", EXIT_SUCCESS);
    return 0;
}