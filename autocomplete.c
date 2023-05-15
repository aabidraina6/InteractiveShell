#include "globals.h"

char *tab_fill(char *curr )
{
    char *cwd = malloc(BUFFERSIZE);
    cwd = getcwd(cwd, BUFFERSIZE);
    struct dirent **namelist;
    struct stat st;
    char **names = malloc(BUFFERSIZE * sizeof(char *));
    for (int i = 0; i < BUFFERSIZE; i++)
        names[i] = malloc(BUFFERSIZE);
    char *toprint = malloc(BUFFERSIZE);
    int n = scandir(cwd, &namelist, 0, alphasort);
    int size = strlen(curr);
    int pt = 0;
    int max_pt = -1;
    int flag = 0;
    char *wpath = malloc(BUFFERSIZE);
    char mode;
    strcpy(wpath, cwd);
    strcat(wpath, "/");
    printf("\n");


    for (int i = 0; i < n; i++)
    {
        //     printf("%s === %s\n",namelist[i]->d_name,curr);
        // printf("inside i = %d\n",strlen(namelist[i]->d_name) < strlen(curr));
        if (strlen(namelist[i]->d_name) < strlen(curr))
            continue;

        // printf("%d\n",pt);

        if (strncmp(namelist[i]->d_name, curr, size))
            continue;
        strcpy(names[pt++], namelist[i]->d_name);

        strcpy(wpath, cwd);
        strcat(wpath, "/");
        strcat(wpath, namelist[i]->d_name);
       
        if (stat(wpath, &st) == 0)
        {
            mode_t perm = st.st_mode;
            mode = S_ISREG(perm) ? '-' : 'd';
            // printf("%c\n",mode);
            if (mode == 'd')
            {
                // printf("inside concat\n");
                strcat(names[pt-1], "/");
                // printf("%s\n",names[pt]);
            }
            else
                strcat(names[pt-1], " ");
        }
        if (strlen(namelist[i]->d_name) == size)
        {

            strcat(wpath, toprint);
            if (stat(wpath, &st) == 0)
            {
                mode_t perm = st.st_mode;
                mode = S_ISREG(perm) ? '-' : 'd';
            }
            strcpy(toprint, namelist[i]->d_name);
            if (mode == 'd')
            {
                strcat(toprint, "/");
            }
            else
                strcat(toprint, " ");

            flag = 1;
        }
        


        int s = strlen(namelist[i]->d_name);
        if (s > max_pt)
        {
            // printf("inside strlen\n");
            max_pt = strlen(namelist[i]->d_name);
        }
    }

    int t = 0;
    if (!flag)
    {
        // printf("%d\n", max_pt);
        for (int i = size; i < max_pt; i++)
        {
            for (int j = 0; j < pt; j++)
            {
                if (names[j][i] == '\0')
                {
                    // printf("first %d\n",i);
                    goto L1;
                }
                if (names[j][i] != names[0][i])

                {
                    // printf("second %d\n",i);

                    goto L1;
                }
            }
            // printf("outside for %c\n", names[0][i]);
            toprint[t++] = names[0][i];
            continue;

        L1:
            break;
        }
    }
    char *d = malloc(BUFFERSIZE);
    strcpy(d, curr);
    strcat(d, toprint);
    int flag2 = 0;
    if (flag == 0)
    {
        for (int i = 0; i < n; i++)
        {
            if (strcmp(namelist[i]->d_name, d) == 0)
            {
                strcpy(wpath, cwd);
                strcat(wpath, "/");
                strcat(wpath, toprint);
                if (stat(wpath, &st) == 0)
                {
                    mode_t perm = st.st_mode;
                    mode = S_ISREG(perm) ? '-' : 'd';
                }
                strcpy(toprint, namelist[i]->d_name);
                if (mode == 'd')
                {
                    strcat(toprint, "/");
                }
                else
                    strcat(toprint, " ");
                flag2 = 1;
                break;
            }
        }
    }
    if ((flag == 0 || pt != 1) && flag2 == 0)
    {
        for (int i = 0; i < pt; i++)
        {
            // printf("inside for \n");
            printf("%s  ", names[i]);
        }

        printf("\n");
    }
    if (pt == 1)
        strcpy(toprint, names[0] + strlen(curr));
    return toprint;
}