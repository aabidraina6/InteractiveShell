#include "globals.h"

int sig_util(int jobid, int signo)
{
    int ret = kill(jobid, signo);
    if (ret < 0)
    {
        perror("Error");
        return -1;
    }
    return 0;
}

int bg_util(int jobid)
{
    if(kill(jobid,SIGCONT)<0)
    {
        perror("Error");
        return -1;
    }
    return 0;
}

int jobs_util(int *bgpid, char **bgCommand, char **args)
{
    char *fname = malloc(BUFFERSIZE);
    char bgno[100];
    int pos = 1;
    char c;
    char status[25];

    int sflag = 0, rflag = 0;

    int i = 0;
    while (args[i])
    {
        if (strcmp(args[i], "-s") == 0)
            sflag = 1;
        if (strcmp(args[i], "-r") == 0)
            rflag = 1;
        i++;
    }

    while (bgpid[pos])
    {
        // printf("inside bgpid\n");
        strcpy(fname, "/proc/");
        sprintf(bgno, "%d", bgpid[pos]);
        strcat(fname, bgno);
        strcat(fname, "/stat");
        FILE *fp = fopen(fname, "r");

        if (fp)
        {
            fscanf(fp, "%*d %*s %c", &c); // reads int and string arguments but doesnot assign them args
            if (c == 'T')
                strcpy(status, "Stopped");
            else
                strcpy(status, "Running");
            if ((sflag && rflag) || (!sflag && !rflag))
            {
                printf("[%d] %s %s [%d]\n", pos, status, bgCommand[pos], bgpid[pos]);
            }
            else
            {
                if (rflag && strcmp(status, "Running") == 0)
                {
                    printf("[%d] %s %s [%d]\n", pos, status, bgCommand[pos], bgpid[pos]);
                }
                if (sflag && strcmp(status, "Stopped") == 0)
                {
                    printf("[%d] %s %s [%d]\n", pos, status, bgCommand[pos], bgpid[pos]);
                }
            }
            // printf("inside fp\n");
        }
        else
        {
            perror("");
            return -1;
        }
        pos++;
        fclose(fp);
    }
    return 0;
}
