#include "globals.h"

void Update(char *cwd,char *root, char *final_path)
{
    // char cwd[1025];
    // getcwd(cwd, 1025);
    int root_len, curr_len, min_len;
    root_len = strlen(root);
    curr_len = strlen(cwd);
    if (curr_len < root_len)
        min_len = curr_len;
    else
        min_len = root_len;

    int itr = 0;
    for (itr = 0; itr < min_len; itr++)
    {
        if (cwd[itr] != root[itr])
            break;
    }
    if (itr == root_len)
    {
        cwd[0] = '~';
        strcpy(&cwd[1], &cwd[root_len]);
        itr += curr_len - root_len - 1;
        cwd[itr] = '\0';
        strcpy(final_path, cwd);
    }
    // printf("after for lloop %s\n",cwd);
    strcpy(final_path, cwd);
    return ;
}

int pinfo_util(char **words, char *root, int n)
{
    int pid_new;
    char status[64];
    int mem;
    char path[1024];
    char name[1024], copy[1024];
    strcpy(name, "/proc");
    if (n == 0)
    {
        strcat(name, "/self");
    }
    else
    {
        // printf("%s\n", words[1]);
        pid_new = atoi(words[1]);
        // strcat(name,pid_new);
        if (kill(pid_new, 0) == -1)
        {
            perror("Invalid PID specification");
            return -1;
        }
        strcat(name, "/");
        strcat(name, words[1]);
    }
    strcpy(copy, name);
    strcat(name, "/stat");
    // printf("\033[0;33m%s\n\033[0;37m", name);
    
    FILE *fp = fopen(name, "r");
    if (fp == NULL)
    {
        perror("error in pinfo");
        return -1;
    }
    fscanf(fp, "%d", &pid_new);
    printf("pid : %d\n", pid_new);

    char pp[100];
    fscanf(fp, "%s", pp);
    char stt[100];
    fscanf(fp, "%s", stt);
    /*
        The function tcgetgrp() returns the pid of the process which is using the 
        the foreground in the terminal
    */
    pid_t console_pid = tcgetpgrp(STDOUT_FILENO);
    if(console_pid==pid_new)
    strcat(stt,"+");
    printf("process Status : %s\n", stt);
    fclose(fp);
    strcat(name, "m");

    FILE *fp1 = fopen(name, "r");
    if (fp1 == NULL)
    {
        perror("error in pinfo");
        return -1;
    }
    fscanf(fp1, "%d", &mem);
    printf("memory : %d\n", mem);
    strcpy(name, copy);
    strcat(name, "/exe");
    // printf("\033[0;33m%s\n\033[0;37m", name);
    fclose(fp1);

    long l = readlink(name, path, 1024);
     if (l < 0)
    {
        perror("error in reading path link");
        return -1;
    }
    path[l] = '\0';
    // printf("%ld\n", l);
    // printf("%s\n",path);
    char *final_path = malloc(1024*sizeof(char));
    Update(path,root, final_path);
    printf("executable path : %s\n", path);
    return -1;
}