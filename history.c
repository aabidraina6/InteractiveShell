#include "globals.h"

void save_history(char *root, char *input)
{

    char *temp = malloc(1024 * sizeof(char));
    char *path = malloc(1024 * sizeof(char));
    char *ip = malloc(1024 * sizeof(char));
    strcpy(ip, input);

    strcpy(path, root);
    strcat(path, "/history.txt");
    FILE *fp = fopen(path, "a+");
    int n = 0;
    fseek(fp, 0, SEEK_SET);
    strcat(ip, "\n");
    while (fgets(temp, 1024, fp))
    {
        if (strcmp(temp, ip) == 0)
            return;
        n++;
    }
    if (n >= 20)
    {
        char *newname = malloc(sizeof(char) * 1024);
        strcpy(newname, root);
        strcat(newname, "/history2.txt");
        FILE *fp_new = fopen(newname, "a");
        fseek(fp, 0, SEEK_SET);
        fgets(temp, 1024, fp);
        while (fgets(temp, 1024, fp))
        {
            fputs(temp, fp_new);
            // fputs("\n", fp_new);
        }
        fputs(ip, fp_new);
        // fputs("\n", fp_new);

        remove(path);
        rename(newname, path);
        fclose(fp);
        fclose(fp_new);
        return;
    }
    else
    {
        fputs(ip, fp);
        // fputs("\n", fp);
        fclose(fp);
        return;
    }
}

void history_print(char *root)
{

    char **op = malloc(21 * sizeof(char *));
    int x = 0;
    char *path = malloc(1024 * sizeof(char));
    for (int i = 0; i < 21; i++)
        op[i] = malloc(1024 * sizeof(char));
    strcpy(path, root);
    strcat(path, "/history.txt");
    FILE *fp = fopen(path, "r");
    while (fgets(op[x++], 1024, fp))
        ;
    for (int i = x - 1; i >= 0; i--)
    {
        if(i<10)
        printf("%s", op[i]);
    }
}