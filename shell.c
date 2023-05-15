#include "globals.h"

#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"
#define SIZE 256

int bgPid[100] = {0};
char *bgCommand[100];

char host_name[65], root[1025], user_name[1025];
int main()
{
    signal(SIGINT,Handler_sig);
    signal(SIGTSTP,Handler_sig);
    signal(SIGCHLD, sigChild_intr);
    struct passwd *user_info = malloc(sizeof(struct passwd));
    uid_t id;
    getcwd(root, 1024);
    gethost_cover(host_name, sizeof(host_name));
    id = getuid_cover();
    user_info = getUserInfo(id);
    char *command = malloc(1025 * sizeof(char));
    char **lines = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        lines[i] = malloc(1025 * sizeof(char));
    char **words = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        words[i] = malloc(1025 * sizeof(char));
    char **seg_ands = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        seg_ands[i] = malloc(1025 * sizeof(char));
    int no_of_segments;
    int no_of_lines;
    int no_of_words;
    ssize_t buffsize = 0;

    int output_fd = 0, input_fd = 0;
    int stdin_copy, stdout_copy;
    strcpy(user_name, user_info->pw_name);

    while (1)
    {
        // printf("%s",BOLD);
        Print_Init(root, host_name, user_info->pw_name);
        int x = 0;
        command = read_line(&x);
        printf("\n");

        lines = split_lines(command, &no_of_lines);

        int l = 0;
        for (int i = 0; i < no_of_lines; i++)
        {
            int bg = 1;
            char *temp_lines = malloc(1024 * sizeof(char));
            char *lines_arg = malloc(1024 * sizeof(char));

            strcpy(temp_lines, lines[i]);
            strcpy(lines_arg, lines[i]);
            seg_ands = split_and(lines_arg, &no_of_segments);
            // printf("no of segments = %d\n",no_of_segments);
            if (1)
            {
                // printf("yes %c\n",temp_lines[strlen(temp_lines) - 1] );
                if (temp_lines[strlen(temp_lines) - 1] != '&')
                    bg = 0;
            }

            for (int j = 0; j < no_of_segments; j++)
            {
                if (no_of_segments > 1)
                    bg = 1;
                if (no_of_segments > 1 && j != no_of_segments - 1)
                    bg = 0;
                // printf("%s\n", seg_ands[j]);
                words = split_words(seg_ands[j], &no_of_words);
                if (x)
                    save_history(root, words[0]);
                int io_mode = -1; //* -1 st , 0 in , 1 out> , 2 out>>
                char **token = malloc(BUFFERSIZE * sizeof(char *));
                for (int i = 0; i < BUFFERSIZE; i++)
                    token[i] = malloc(BUFFERSIZE);
                int token_size = 0;

                char *file = malloc(BUFFERSIZE);

                input_fd = 0;
                output_fd = 0;

                for (int i = 0; i < no_of_words; i++)
                {
                    if (io_mode > -1)
                    {
                        if (io_mode)
                        {
                            char ttemp[BUFFERSIZE];
                            getcwd(ttemp, BUFFERSIZE);
                            strcat(ttemp, "/");
                            strcat(ttemp, words[i]);
                            // printf("%s\n", ttemp);

                            if (io_mode == 1)
                                output_fd = open(ttemp, O_WRONLY | O_CREAT | O_APPEND, 0644);
                            else if (io_mode = 2)
                                output_fd = open(ttemp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        }
                        else
                        {
                            char ttemp[BUFFERSIZE];
                            getcwd(ttemp, BUFFERSIZE);
                            strcat(ttemp, "/");

                            strcat(ttemp, words[i]);
                            // close (input_fd);
                            input_fd = open(ttemp, O_RDONLY);
                        }
                        io_mode = -1;
                    }

                    else
                    {
                        if (strcmp(words[i], "<") == 0)
                        {
                            io_mode = 0;
                        }
                        else if (strcmp(words[i], ">") == 0)
                        {
                            io_mode = 1;
                        }
                        else if (strcmp(words[i], ">>") == 0)
                        {
                            io_mode = 2;
                        }
                        else
                        {
                            strcpy(token[token_size++], words[i]);
                        }
                    }
                }

                if (input_fd < 0 || output_fd < 0)
                {
                    perror("nd");
                    continue;
                }
                if (input_fd)
                {
                    stdin_copy = dup(0);
                    dup2(input_fd, STDIN_FILENO);
                }

                if (output_fd)
                {
                    stdout_copy = dup(1);
                    dup2(output_fd, STDOUT_FILENO);
                }

                if (strcmp(token[0], "cd") == 0)
                {

                    cd_util(token, root);
                }
                else if (strcmp(token[0], "echo") == 0)
                {
                    echo_util(token, token_size);
                }

                else if (strcmp(token[0], "pwd") == 0)
                {
                    pwd_util();
                }
                else if (strcmp(token[0], "ls") == 0)
                {
                    ls_caller(token, root, token_size);
                }
                else if (strcmp(token[0], "pinfo") == 0)
                {
                    int z;
                    if (strlen(token[1]) == 0)
                        z = 0;
                    else
                        z = 1;
                    pinfo_util(token, root, z);
                }
                else if (strcmp(token[0], "discover") == 0)
                {
                    {
                        char *target = malloc(1024 * sizeof(char));
                        char *file_name = malloc(1024 * sizeof(char));
                        strcpy(target, "/.");
                        // printf("%s\n",target);
                        int flag = 0;
                        int flag_f = 0;
                        int flag_d = 0;
                        for (int i = 1; i < token_size; i++)
                        {
                            // printf("%s\n",token[i]);
                            if (strcmp(token[i], "-f") == 0)
                                flag_f = 1;
                            else if (strcmp(token[i], "-d") == 0)
                                flag_d = 1;
                            else if (token[i][0] == '\"')
                            {
                                strcpy(target, token[i]);
                                // printf("%d\n",strlen(target)-1);
                                target[strlen(target) - 1] = '\0';
                                // printf("token[i] === %s\n",&target[1]);
                            }

                            else
                            {
                                flag = 1;
                                strcpy(file_name, token[i]);
                            }
                        }
                        // printf("%s\n",&target[1]);
                        disc_util(root, flag_d, flag_f, &target[1], file_name, &target[1], flag);
                    }
                }
                else if (strcmp(token[0], "history") == 0)
                    history_print(root);
                else if (strcmp(token[0], "jobs") == 0)
                    jobs_util(bgPid, bgCommand, token);
                else if (strcmp(token[0], "sig") == 0)
                {
                    if (no_of_words != 3)
                        perror("Invalid number of arguments");
                    else
                    {
                        int jobid = atoi(token[1]);
                        int hand = atoi(token[2]);
                        if (jobid < 1 || jobid > 1024 || bgPid[jobid] <= 1)
                        {
                            printf("Error: Job not found\n ");
                        }
                        sig_util(bgPid[jobid], hand);
                    }
                }
                else if (strcmp(token[0], "bg") == 0)
                {
                    if (no_of_words < 2)
                    {
                        printf("Error:Too few arguments\n");
                    }
                    else
                    {
                        int jobid = atoi(token[1]);
                        bg_util(bgPid[jobid]);
                    }
                }
                else if(strcmp(token[0],"exit")==0)
                exit(0);
                else
                {
                    int pos = 1;
                    // for (int i = 0;i<no_of_token;i++)
                    // printf("%s\n",token[i]);
                    // printf("%d\n",bg);
                    char temp_command[1024];
                    strcpy(temp_command, token[0]);
                    token[token_size] = NULL;
                    int pid_ret = runCommand(bg, token, pos);
                    if (pid_ret < 0)
                    {
                        perror("");
                    }
                    else if (pid_ret > 0)
                    {
                        pos = addBg(pid_ret, temp_command);

                        // Print_Init(root, host_name, user_info->pw_name);
                    }
                }
                if (input_fd)
                {
                    close(input_fd);
                    dup2(stdin_copy, STDIN_FILENO);
                    close(stdin_copy);
                }
                if (output_fd)
                {
                    close(output_fd);

                    dup2(stdout_copy, STDOUT_FILENO);
                    close(stdout_copy);
                }
                // input_fd = output_fd = 0;
            }
        }
    }

    return 0;
}