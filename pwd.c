#include"globals.h"

int pwd_util()
{
    char *temp = malloc(1025 * sizeof(char));
    getcwd(temp, 1025);
    printf("\033[0;035m%s\n", temp);
    free(temp);

    return 1;
}