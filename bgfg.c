#include <termios.h>

#include "globals.h"

extern int bgPid[100];
extern char *bgCommand[100];

void sigChild_intr()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        int pos = 0;
        while (bgPid[pos] != pid)
            pos++;

        if (WIFSTOPPED(status))
            return;

        printf("%s with PID = %d exited %s\n", bgCommand[pos], bgPid[pos],
               WIFEXITED(status) ? "normally" : "abnormally");

        bgPid[pos] = 0;
        free(bgCommand[pos]);
    }
}

int runCommand(int background, char **args, int pos)
{
    // printf("%s\n",args[0]);
    int pid = fork();

    if (pid < 0)
    {
        perror("Error");
        return -1;
    }
    else if (pid)
    {
        if (background)
        {
            printf("[%d] %d\n", pos, pid);

            return pid;
        }
        else
        {
            int status;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(0, pid);

            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(0, getpgid(0));

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status))
                return pid;

            if (WEXITSTATUS(status) == EXIT_FAILURE)
                return -1;
        }
    }
    else
    {
        setpgid(0, 0);
        // printf("inside %s\n",args[1]);
        if (execvp(args[0], args) < 0)
        {
            perror("Error");
            exit(1);
        }
        // printf("after exec\n");
    }

    return 0;
}

int addBg(int pid, char *command)
{
    // printf("inside addbg\n");
    if (!pid)
        return 0;

    int pos = 99;

    while (!bgPid[pos] && pos > 0)
        pos--;
    // printf("after while %d\n",pos);

    pos++;

    bgPid[pos] = pid;
    // printf("before mlloc %d\n",(strlen(command) + 1));
    bgCommand[pos] = malloc((1024) * sizeof(char));
    // printf("com %s\n",command);
    // printf("after malloc\n");
    strcpy(bgCommand[pos], command);
    return pos;
}

void removeBg(int pid)
{
    if (!pid)
        return;

    for (int i = 1; i < 100; i++)
    {
        if (bgPid[i] == pid)
        {
            bgPid[i] = 0;
            free(bgCommand[i]);
            break;
        }
    }
}