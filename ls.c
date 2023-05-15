#include <dirent.h>
#include <sys/stat.h>
#include "globals.h"

void ls_caller(char **words, char *root, int count_words)
{
    int flag_a = 0, flag_l = 0;
    int flag_tilde = 0, flag_dot = 0;
    char **dir_names = malloc(BUFFERSIZE * sizeof(char *));
    int n = 0;
    char cwd[1025];
    char cwd_rip[1025];
    getcwd(cwd, sizeof(cwd));
    strcpy(cwd_rip, cwd);
    char dir[BUFFERSIZE][BUFFERSIZE];
    char *temp = malloc(BUFFERSIZE * sizeof(char));
    temp = strtok(cwd_rip, "/");
    int count = 0;
    while (temp)
    {
        strcpy(dir[count++], temp);
        temp = strtok(NULL, "/");
    }

    for (int i = 0; i < count_words; i++)
    {
        if (strcmp(words[i], "-l") == 0)
            flag_l = 1;
        else if (strcmp(words[i], "-a") == 0)
            flag_a = 1;
        else if (strcmp(words[i], "-la") == 0 || strcmp(words[i], "-al") == 0)
            flag_a = flag_l = 1;
        else if (strcmp(words[i], "..") == 0)
            flag_dot = 1;
        else if (strcmp(words[i], "~") == 0)
            flag_tilde = 1;
        else
        {
            if (strcmp(words[i], "ls") == 0)
                continue;
            if (strcmp(words[i], "\n") == 0)
                continue;
            if (strcmp(words[i], " ") == 0)
                continue;
            dir_names[n] = malloc(BUFFERSIZE * sizeof(char));
            strcpy(dir_names[n++], words[i]);
        }
    }
    if (flag_a)
    {
        if (flag_l)
        {
            if (flag_dot)
            {
                char *name = malloc(BUFFERSIZE * sizeof(char));
                for (int i = 0; i < count - 1; i++)
                {
                    strcat(name, "/");
                    strcat(name, dir[i]);
                }
                lsl_util(1, name, dir_names, n);
                //!! get the count of files..
            }
            else if (flag_tilde)
            {
                lsl_util(1, root, dir_names, n);
            }
            else
            {
                lsl_util(1, cwd, dir_names, n);
            }
        }
        else
        {
            if (flag_dot)
            {
                char *name = malloc(BUFFERSIZE * sizeof(char));
                for (int i = 0; i < count - 1; i++)
                {
                    strcat(name, "/");
                    strcat(name, dir[i]);
                }
                ls_util(1, name, dir_names, n);
                //!! get the count of files..
            }
            else if (flag_tilde)
            {
                ls_util(1, root, dir_names, n);
            }
            else
            {
                ls_util(1, cwd, dir_names, n);
            }
        }
    }
    else if (flag_l)
    {
        if (flag_dot)
        {
            char *name = malloc(BUFFERSIZE * sizeof(char));
            for (int i = 0; i < count - 1; i++)
            {
                strcat(name, "/");
                strcat(name, dir[i]);
            }
            lsl_util(0, name, dir_names, n);
            //!! get the count of files..
        }
        else if (flag_tilde)
        {
            lsl_util(0, root, dir_names, n);
        }
        else
        {
            lsl_util(0, cwd, dir_names, n);
        }
    }
    else
    {
        if (flag_dot)
        {
            char *name = malloc(BUFFERSIZE * sizeof(char));
            for (int i = 0; i < count - 1; i++)
            {
                strcat(name, "/");
                strcat(name, dir[i]);
            }
            ls_util(0, name, dir_names, n);
            //!! get the count of files..
        }
        else if (flag_tilde)
        {
            ls_util(0, root, dir_names, n);
        }
        else
        {
            // printf("inside elswe\n");
            ls_util(0, cwd, dir_names, n);
        }
    }
}

int ls_util(int flag, char *name, char **dir_names, int count)
{
    char files[BUFFERSIZE][BUFFERSIZE];
    char directories[BUFFERSIZE][BUFFERSIZE];
    int no_files = 0;
    int no_dir = 0;

    for (int i = 0; i < count; i++)
    {
        struct stat sts;
        char nn[1025];
        nn[0] = '\0';
        strcat(nn, name);
        strcat(nn, "/");
        strcat(nn, dir_names[i]);
        int stt = stat(nn, &sts);
        mode_t permss = sts.st_mode;
        char dd =
            S_ISREG(permss) ? '-' : 'd';
        if (dd == 'd')
            strcpy(directories[no_dir++], dir_names[i]);
        else
            strcpy(files[no_files++], dir_names[i]);
    }
    int x = 0, y = 0;
    for (int i = 0; i < count; i++)
    {
        if (i < no_files)
            strcpy(dir_names[i], files[x++]);
        else
            strcpy(dir_names[i], directories[y++]);
    }
    // for  (int i = 0;i<count;i++)
    // {
        // printf("%s\n",dir_names[i]);
    // }
    struct dirent **namelist;
    // flag determines whether hidden files are to be shown.
    int n = scandir(name, &namelist, 0, alphasort);
    if (n == -1)
    {
        perror("ls");
        return -1;
    }
    int flag_found = 0;
    int flag_gtr = 0;
    int flag_first = 0;

    if (count > 1)
        flag_gtr = 1;
    if (count > 0)
    {
        struct stat st;

        for (int j = 0; j < count; j++)
        {
            flag_found = 0;
            for (int i = 0; i < n; i++)
            {
                if (strcmp(dir_names[j], namelist[i]->d_name) == 0)
                {
                    // if (flag == 0 && dir_names[j][0] == '.')
                    //     continue;
                    flag_found = 1;
                    char name_new[1025];
                    name_new[0] = '\0';
                    strcat(name_new, name);
                    strcat(name_new, "/");
                    strcat(name_new, dir_names[j]);
                    if (stat(name_new, &st) == 0)
                    {
                        mode_t perm;
                        perm = st.st_mode;
                        char mode = S_ISREG(perm) ? '-' : 'd';
                        char is_ex = (perm & S_IXUSR) ? 'x' : '-';
                        if (mode == 'd')
                        {
                            if (flag_gtr)
                            {
                                if (flag_first == 1)
                                    printf("\n");
                                flag_first = 1;
                                printf("\033[0;33m%s:\n\033[0;37m", dir_names[j]);
                            }
                            ls_util(flag, name_new, dir_names, -1);
                        }
                        else

                        {
                            if (is_ex == 'x')
                                printf("\033[0;32m%s \033[0;37m", dir_names[j]);
                            else
                                printf("%s ", dir_names[j]);

                            flag_first = 1;
                        }
                    }
                    else
                    {
                        perror("stat");
                        return -1;
                    }
                }
            }
            // free(namelist[i]);
            if (flag_found == 0)
                printf("ls: cannot access '%s':No such file or directory\n", dir_names[j]);
        }
        printf("\n");
        // free(namelist);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            struct stat st;
            char *newname = malloc(BUFFERSIZE * sizeof(char));
            newname[0] = '\0';
            strcpy(newname, name);
            strcat(newname, "/");
            strcat(newname, namelist[i]->d_name);
            int stat_ret = stat(newname, &st);
            mode_t perm = st.st_mode;
            char mode_d = S_ISREG(perm) ? '-' : 'd';
            char is_ex = (perm & S_IXUSR) ? 'x' : '-';

            if (flag == 0 && namelist[i]->d_name[0] == '.')
                continue;
            if (mode_d == 'd')
                printf("\033[0;34m%s \033[0;37m", namelist[i]->d_name);
            else if (is_ex == 'x')
                printf("\033[0;32m%s \033[0;37m", namelist[i]->d_name);
            else
                printf("%s ", namelist[i]->d_name);

            free(namelist[i]);
        }
        printf("\n");
        free(namelist);
    }
    return n;

    // this utility will return the number of files in the directory
}


int count_blocks(int flag, char *name, char **dir_names, int count)
{
    int ans = 0;
    char files[BUFFERSIZE][BUFFERSIZE];
    char directories[BUFFERSIZE][BUFFERSIZE];
    int no_files = 0;
    int no_dir = 0;

    for (int i = 0; i < count; i++)
    {
        struct stat sts;
        char nn[1025];
        nn[0] = '\0';
        strcat(nn, name);
        strcat(nn, "/");
        strcat(nn, dir_names[i]);
        int stt = stat(nn, &sts);
        mode_t permss = sts.st_mode;
        char dd =
            S_ISREG(permss) ? '-' : 'd';
        if (dd == 'd')
            strcpy(directories[no_dir++], dir_names[i]);
        else
            strcpy(files[no_files++], dir_names[i]);
    }
    int x = 0, y = 0;
    for (int i = 0; i < count; i++)
    {
        if (i < no_files)
            strcpy(dir_names[i], files[x++]);
        else
            strcpy(dir_names[i], directories[y++]);
    }
    // printf("inside lsl\n");
    struct dirent **namelist;
    struct stat st;
    char *modes = (char *)calloc(11, sizeof(char));
    char *currdir = calloc(1000, 1);
    strcpy(currdir, name);
    int n = scandir(name, &namelist, 0, alphasort);
    if (n == -1)
    {
        perror("ls");
        return -1;
    }
    int flag_found = 0;
    int flag_gtr = 0;
    int flag_first = 0;
    if (count > 1)
        flag_gtr = 1;
    // for (int i = 0;i<n;i++)
    // printf("%s\n",namelist[i]->d_name);
    if (count > 0)
    {
        for (int j = 0; j < count; j++)
        {
            flag_found = 0;
            for (int i = 0; i < n; i++)
            {
                if (strcmp(dir_names[j], namelist[i]->d_name) == 0)
                {
                    flag_found = 1;

                    {
                        char *temp = calloc(1000, 1);
                        temp[0] = '\0';
                        // printf("%s\n",tempname);
                        strcat(temp, currdir);
                        // printf("\033[0;31m%s\n",temp);

                        strcat(temp, "/");
                        // printf("\033[0;31m%s\n",temp);

                        strcat(temp, namelist[i]->d_name);
                        // printf("\033[0;31m%s\n",temp);

                        // printf("\033[0;31m%s\n",temp);
                        // printf("\033[0;37m");
                        if (stat(temp, &st) == 0)

                        {
                            // strcpy(temp,"");
                            temp[0] = '\0';
                            // printf("%s\n",temp);
                            struct passwd *user_info = malloc(sizeof(struct passwd));
                            struct group *group_info = malloc(sizeof(struct passwd));

                            mode_t perm = st.st_mode;
                            user_info = getpwuid(st.st_uid);
                            group_info = getgrgid(st.st_uid);
                            char time[200];
                            
                            strftime(time, sizeof(time), "%b %d %H:%M", localtime(&st.st_mtime));

                            modes[0] = S_ISREG(perm) ? '-' : 'd';
                            modes[1] = (perm & S_IRUSR) ? 'r' : '-';
                            modes[2] = (perm & S_IWUSR) ? 'w' : '-';
                            modes[3] = (perm & S_IXUSR) ? 'x' : '-';
                            modes[4] = (perm & S_IRGRP) ? 'r' : '-';
                            modes[5] = (perm & S_IWGRP) ? 'w' : '-';
                            modes[6] = (perm & S_IXGRP) ? 'x' : '-';
                            modes[7] = (perm & S_IROTH) ? 'r' : '-';
                            modes[8] = (perm & S_IWOTH) ? 'w' : '-';
                            modes[9] = (perm & S_IXOTH) ? 'x' : '-';
                            modes[10] = '\0';
                            // printf("\033[0;34m %d\n",st.st_blocks);
                            if (modes[0] == 'd')
                            {
                                char name_new[1025] = "";

                                strcat(name_new, name);
                                strcat(name_new, "/");
                                strcat(name_new, namelist[i]->d_name);
                                if (flag_first == 1)
                                    // printf("\n");
                                flag_first = 1;
                                if (flag_gtr)
                                    // printf("\033[0;33m%s:\n\033[0;37m", dir_names[j]);

                                lsl_util(flag, name_new, dir_names, -1);
                            }
                            else
                            {
                                flag_first = 1;

                                ans +=st.st_blocks;
                                // printf("%5s ", modes);
                                // printf("%3ld ", st.st_nlink);
                                // printf("%5s ", user_info->pw_name);
                                // printf("%5s ", group_info->gr_name);
                                // printf("%5ld ", st.st_size);
                                // printf("%5s ", time);
                                // if (modes[0] == 'd')
                                    // printf("\033[0;34m%s\n\033[0;37m", namelist[i]->d_name);
                                // else if (modes[3] == 'x')
                                    // printf("\033[0;32m%s\n\033[0;37m", namelist[i]->d_name);
                                // else
                                    // printf("%s\n", namelist[i]->d_name);
                            }
                            // free(namelist[i]);
                        }
                        else
                        {
                            perror("stat");
                            return -1;
                        }
                    }
                }
                // free(namelist[i]);
            }
            // if (flag_found == 0)
                // printf("ls: cannot access '%s' :No such file or directory\n", dir_names[j]);
        }
    }
    else

    {

        for (int i = 0; i < n; i++)
        {
            if (flag == 0 && namelist[i]->d_name[0] == '.')
                continue;
            else
            {
                char *temp = calloc(1000, 1);
                // printf("%s\n",tempname);
                strcat(temp, currdir);
                // printf("\033[0;31m%s\n",temp);

                strcat(temp, "/");
                // printf("\033[0;31m%s\n",temp);

                strcat(temp, namelist[i]->d_name);
                // printf("\033[0;31m%s\n",temp);

                // printf("\033[0;31m%s\n",temp);
                // printf("\033[0;37m");
                if (stat(temp, &st) == 0)

                {

                    // strcpy(temp,"");
                    temp[0] = '\0';
                    // printf("%s\n",temp);
                    struct passwd *user_info = malloc(sizeof(struct passwd));
                    struct group *group_info = malloc(sizeof(struct passwd));

                    mode_t perm = st.st_mode;
                    user_info = getpwuid(st.st_uid);
                    group_info = getgrgid(st.st_uid);
                    char time[200];
                    strftime(time, sizeof(time), "%b %d %H:%M", localtime(&st.st_mtime));

                    modes[0] = S_ISREG(perm) ? '-' : 'd';
                    modes[1] = (perm & S_IRUSR) ? 'r' : '-';
                    modes[2] = (perm & S_IWUSR) ? 'w' : '-';
                    modes[3] = (perm & S_IXUSR) ? 'x' : '-';
                    modes[4] = (perm & S_IRGRP) ? 'r' : '-';
                    modes[5] = (perm & S_IWGRP) ? 'w' : '-';
                    modes[6] = (perm & S_IXGRP) ? 'x' : '-';
                    modes[7] = (perm & S_IROTH) ? 'r' : '-';
                    modes[8] = (perm & S_IWOTH) ? 'w' : '-';
                    modes[9] = (perm & S_IXOTH) ? 'x' : '-';
                    modes[10] = '\0';
                    // printf("\033[0;34m %d\n",st.st_blocks);
                    

                    // printf("%5s ", modes);
                    // printf("%3ld ", st.st_nlink);
                    // printf("%5s ", user_info->pw_name);
                    // printf("%5s ", group_info->gr_name);
                    // printf("%5ld ", st.st_size);
                    // printf("%5s ", time);
                    ans+=st.st_blocks;

                    // if (modes[0] == 'd')
                    //     // printf("\033[0;34m%s\n\033[0;37m", namelist[i]->d_name);
                    // else if (modes[3] == 'x')
                    //     printf("\033[0;32m%s\n\033[0;37m", namelist[i]->d_name);
                    // else
                    //     printf("%s\n", namelist[i]->d_name);
                    // free(namelist[i]);
                }
                else
                {
                    // perror("stat");
                    return -1;
                }
            }
        }
    }
    return ans;
}



int lsl_util(int flag, char *name, char **dir_names, int count)
{
    // int blocks_count  = count_blocks(flag,name,dir_names,count);
    // printf("total %d\n",blocks_count);
    char files[BUFFERSIZE][BUFFERSIZE];
    char directories[BUFFERSIZE][BUFFERSIZE];
    int no_files = 0;
    int no_dir = 0;

    for (int i = 0; i < count; i++)
    {
        struct stat sts;
        char nn[1025];
        nn[0] = '\0';
        strcat(nn, name);
        strcat(nn, "/");
        strcat(nn, dir_names[i]);
        int stt = stat(nn, &sts);
        mode_t permss = sts.st_mode;
        char dd =
            S_ISREG(permss) ? '-' : 'd';
        if (dd == 'd')
            strcpy(directories[no_dir++], dir_names[i]);
        else
            strcpy(files[no_files++], dir_names[i]);
    }
    int x = 0, y = 0;
    for (int i = 0; i < count; i++)
    {
        if (i < no_files)
            strcpy(dir_names[i], files[x++]);
        else
            strcpy(dir_names[i], directories[y++]);
    }
    // printf("inside lsl\n");
    struct dirent **namelist;
    struct stat st;
    char *modes = (char *)calloc(11, sizeof(char));
    char *currdir = calloc(1000, 1);
    strcpy(currdir, name);
    int n = scandir(name, &namelist, 0, alphasort);
    if (n == -1)
    {
        perror("ls");
        return -1;
    }
    int flag_found = 0;
    int flag_gtr = 0;
    int flag_first = 0;
    if (count > 1)
        flag_gtr = 1;
    // for (int i = 0;i<n;i++)
    // printf("%s\n",namelist[i]->d_name);
    if (count > 0)
    {
        for (int j = 0; j < count; j++)
        {
            flag_found = 0;
            for (int i = 0; i < n; i++)
            {
                if (strcmp(dir_names[j], namelist[i]->d_name) == 0)
                {
                    flag_found = 1;

                    {
                        char *temp = calloc(1000, 1);
                        temp[0] = '\0';
                        // printf("%s\n",tempname);
                        strcat(temp, currdir);
                        // printf("\033[0;31m%s\n",temp);

                        strcat(temp, "/");
                        // printf("\033[0;31m%s\n",temp);

                        strcat(temp, namelist[i]->d_name);
                        // printf("\033[0;31m%s\n",temp);

                        // printf("\033[0;31m%s\n",temp);
                        // printf("\033[0;37m");
                        if (stat(temp, &st) == 0)

                        {
                            // strcpy(temp,"");
                            temp[0] = '\0';
                            // printf("%s\n",temp);
                            struct passwd *user_info = malloc(sizeof(struct passwd));
                            struct group *group_info = malloc(sizeof(struct passwd));

                            mode_t perm = st.st_mode;
                            user_info = getpwuid(st.st_uid);
                            group_info = getgrgid(st.st_uid);
                            char time[200];
                            
                            strftime(time, sizeof(time), "%b %d %H:%M", localtime(&st.st_mtime));

                            modes[0] = S_ISREG(perm) ? '-' : 'd';
                            modes[1] = (perm & S_IRUSR) ? 'r' : '-';
                            modes[2] = (perm & S_IWUSR) ? 'w' : '-';
                            modes[3] = (perm & S_IXUSR) ? 'x' : '-';
                            modes[4] = (perm & S_IRGRP) ? 'r' : '-';
                            modes[5] = (perm & S_IWGRP) ? 'w' : '-';
                            modes[6] = (perm & S_IXGRP) ? 'x' : '-';
                            modes[7] = (perm & S_IROTH) ? 'r' : '-';
                            modes[8] = (perm & S_IWOTH) ? 'w' : '-';
                            modes[9] = (perm & S_IXOTH) ? 'x' : '-';
                            modes[10] = '\0';
                            // printf("\033[0;34m %d\n",st.st_blocks);
                            if (modes[0] == 'd')
                            {
                                char name_new[1025] = "";

                                strcat(name_new, name);
                                strcat(name_new, "/");
                                strcat(name_new, namelist[i]->d_name);
                                if (flag_first == 1)
                                    printf("\n");
                                flag_first = 1;
                                if (flag_gtr)
                                    printf("\033[0;33m%s:\n\033[0;37m", dir_names[j]);

                                lsl_util(flag, name_new, dir_names, -1);
                            }
                            else
                            {
                                flag_first = 1;

                                printf("%5s ", modes);
                                printf("%3ld ", st.st_nlink);
                                printf("%5s ", user_info->pw_name);
                                printf("%5s ", group_info->gr_name);
                                printf("%5ld ", st.st_size);
                                printf("%5s ", time);
                                if (modes[0] == 'd')
                                    printf("\033[0;34m%s\n\033[0;37m", namelist[i]->d_name);
                                else if (modes[3] == 'x')
                                    printf("\033[0;32m%s\n\033[0;37m", namelist[i]->d_name);
                                else
                                    printf("%s\n", namelist[i]->d_name);
                            }
                            // free(namelist[i]);
                        }
                        else
                        {
                            perror("stat");
                            return -1;
                        }
                    }
                }
                // free(namelist[i]);
            }
            if (flag_found == 0)
                printf("ls: cannot access '%s' :No such file or directory\n", dir_names[j]);
        }
    }
    else

    {

        for (int i = 0; i < n; i++)
        {
            if (flag == 0 && namelist[i]->d_name[0] == '.')
                continue;
            else
            {
                char *temp = calloc(1000, 1);
                // printf("%s\n",tempname);
                strcat(temp, currdir);
                // printf("\033[0;31m%s\n",temp);

                strcat(temp, "/");
                // printf("\033[0;31m%s\n",temp);

                strcat(temp, namelist[i]->d_name);
                // printf("\033[0;31m%s\n",temp);

                // printf("\033[0;31m%s\n",temp);
                // printf("\033[0;37m");
                if (stat(temp, &st) == 0)

                {

                    // strcpy(temp,"");
                    temp[0] = '\0';
                    // printf("%s\n",temp);
                    struct passwd *user_info = malloc(sizeof(struct passwd));
                    struct group *group_info = malloc(sizeof(struct passwd));

                    mode_t perm = st.st_mode;
                    user_info = getpwuid(st.st_uid);
                    group_info = getgrgid(st.st_uid);
                    char time[200];
                    strftime(time, sizeof(time), "%b %d %H:%M", localtime(&st.st_mtime));

                    modes[0] = S_ISREG(perm) ? '-' : 'd';
                    modes[1] = (perm & S_IRUSR) ? 'r' : '-';
                    modes[2] = (perm & S_IWUSR) ? 'w' : '-';
                    modes[3] = (perm & S_IXUSR) ? 'x' : '-';
                    modes[4] = (perm & S_IRGRP) ? 'r' : '-';
                    modes[5] = (perm & S_IWGRP) ? 'w' : '-';
                    modes[6] = (perm & S_IXGRP) ? 'x' : '-';
                    modes[7] = (perm & S_IROTH) ? 'r' : '-';
                    modes[8] = (perm & S_IWOTH) ? 'w' : '-';
                    modes[9] = (perm & S_IXOTH) ? 'x' : '-';
                    modes[10] = '\0';
                    // printf("\033[0;34m %d\n",st.st_blocks);
                    

                    printf("%5s ", modes);
                    printf("%3ld ", st.st_nlink);
                    printf("%5s ", user_info->pw_name);
                    printf("%5s ", group_info->gr_name);
                    printf("%5ld ", st.st_size);
                    printf("%5s ", time);

                    if (modes[0] == 'd')
                        printf("\033[0;34m%s\n\033[0;37m", namelist[i]->d_name);
                    else if (modes[3] == 'x')
                        printf("\033[0;32m%s\n\033[0;37m", namelist[i]->d_name);
                    else
                        printf("%s\n", namelist[i]->d_name);
                    free(namelist[i]);
                }
                else
                {
                    perror("stat");
                    return -1;
                }
            }
        }
    }
}