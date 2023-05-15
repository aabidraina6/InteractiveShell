#include"globals.h"

int cd_util(char **command, char *root)
{
    char cwd[1025];
    getcwd(cwd, 1025);
    char dirs_curr[64][1025];
    char *temp = malloc(1025 * sizeof(char));
    temp = strtok(cwd, "/");
    int count = 0;
    while (temp)
    {
        strcpy(dirs_curr[count++], temp);
        temp = strtok(NULL, "/");
    }

    char *final_cwd = malloc(1025 * sizeof(char));
    strcat(final_cwd, "/");
    for (int i = 0; i < count - 1; i++)
    {
        strcat(final_cwd, dirs_curr[i]);
        strcat(final_cwd, "/");
    }
    char homedir[1025];
    strcpy(homedir, root);
    if ((strcmp(command[1], "~") == 0 || strcmp(command[1], "~/") == 0))
    {
        // printf("inside if\n");
        chdir(homedir);
    }
    else if (strcmp(command[1], "-") == 0)
    {
        chdir(homedir);
        pwd_util();
    }
    else if (strcmp(command[1], "..") == 0)
    {
        // printf("%s\n", final_cwd);

        chdir(final_cwd);
    }
    else
    {
        // printf("inside else\n");
        int res = chdir(command[1]);
        if (res < 0)
            perror("Error");
    }
    return 1;
}