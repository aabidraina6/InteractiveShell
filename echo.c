#include"globals.h"

int echo_util(char **message, int n)
{
    for (int i = 1; i<n; i++)
    {
        printf("%s ", message[i]);
    }
    printf("\n");
    return 1;
}