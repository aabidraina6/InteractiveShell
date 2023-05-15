#include "globals.h"

int disc_util(char *root, int flag_d, int flag_f, char *target, char *file_name, char *to_print, int count)
{

    // if (count==0)
    {
        char *save = malloc(1024 * sizeof(char));
        strcpy(save, target);
        char *tt = malloc(1024 * sizeof(char));
        struct dirent **namelist;
        if (strcmp(target, "~") == 0 || strcmp(target, "~/") == 0)
            strcpy(tt, root);
        else
            strcpy(tt, target);
        // printf("\033[0;31m%s\n\033[0;37m", tt);
        int n = scandir(tt, &namelist, 0, alphasort);
        char cwd[1024];
        getcwd(cwd, 1024);
        if (n == -1)
        {
            perror("invalid path");
            return -1;
        }
        for (int i = 0; i < n; i++)
        {
            struct stat st;
            char *final_name = malloc(sizeof(char) * 1024);
            char *name_work = malloc(sizeof(char) * 1024);
            strcpy(name_work, "");
            strcpy(final_name, "");
            strcat(final_name, to_print);
            strcpy(name_work, target);
            if (strcmp(target, ".") == 0)
            {
                strcpy(name_work, cwd);
                if (to_print[strlen(to_print) - 1] != '/')
                    strcat(to_print, "/");
            }
            int x = 0;
            int size = strlen(cwd);
            for (int i = size - 2; i >= 0; i--)
            {
                x++;
                if (cwd[i] == '/')
                    break;
            }
            // printf("%d\n", size);
            if (strcmp(target, "..") == 0)
            {
                cwd[size - x] = '\0';
                strcpy(name_work, cwd);
                // printf("%s\n", name_work);
                strcat(to_print, "/");
            }
            if (target[0] == '~')
            {
                strcpy(name_work, root);
                if (to_print[strlen(to_print) - 1] != '/')
                    strcat(to_print, "/");
            }

            int flag_slash = 0;
            if (final_name[strlen(final_name) - 1] == '/')
                flag_slash = 0;
            else
                flag_slash = 1;
            if (flag_slash == 1)
                strcat(final_name, "/");
            strcat(final_name, namelist[i]->d_name);
            if (name_work[strlen(name_work) - 1] != '/')
                strcat(name_work, "/");
            // printf("%s\n",name_work);
            strcat(name_work, namelist[i]->d_name);
            // printf("\033[0;36m%s\n\033[0;37m", name_work);
            int st_ret = stat(name_work, &st);
            // printf("%d  ",st_ret);
            mode_t perm = st.st_mode;
            char dd = S_ISREG(perm) ? '-' : 'd';
            if (st_ret == -1)
                dd = '=';
            // printf("%c\n",dd);
            if ((flag_d && dd == 'd') ||
                (flag_f && dd == '-') || (!flag_d && !flag_f) || (flag_d) || flag_f)
            {
                if (namelist[i]->d_name[0] == '.')
                    continue;

                {
                    // if(strcmp(final_name,"/home/aabid/OSN/2021101136_assignment2/test/dummy")==0)
                    // printf("\033[0;31m%s\n\033[0;37m", namelist[i]->d_name);

                    if (count == 1 && strcmp(namelist[i]->d_name, file_name) != 0)
                    {
                    }
                    else
                    {
                        if((flag_d && dd == 'd') ||(flag_f && dd == '-') || (!flag_d && !flag_f))
                        printf("%s\n", final_name);
                    }
                    // printf("\033[0;33m%s\n\033[0;37m", to_print);
                    if (dd == 'd')
                    {
                        if (to_print[strlen(to_print) - 1] != '/')
                            strcat(to_print, "/");
                        strcat(to_print, namelist[i]->d_name);
                        // if(strcmp(name_work,"test.c")==0)
                        // printf("%s-->%c\n",name_work,dd);
                        // strcat()
                        // printf("\033[0;35m%s\n\033[0;37m", name_work);

                        disc_util(root, flag_d, flag_f, name_work, file_name, to_print, count);
                        // int s = strlen(name_work);
                        // x = 0;
                        // for (int i = s - 2; i >= 0; i--)
                        // {
                        //     x++;
                        //     if (name_work[i] == '/')
                        //         break;
                        // }
                        // name_work[s - x] = '\0';
                        strcpy(target, save);
                    }
                }
            }
        }
    }
}