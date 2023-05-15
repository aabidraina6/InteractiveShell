#include "globals.h"

extern char host_name[65], root[1025], user_name[1025];

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any commandut that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

char **split_lines(char *command, int *count)
{
    char *temp;
    char **ret_commands = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        ret_commands[i] = malloc(1025 * sizeof(char));
    temp = strtok(command, ";");
    int i = 0;
    for (i = 0; temp != NULL; ++i)
    {
        strcpy(ret_commands[i], temp);
        temp = strtok(NULL, ";");
    }
    *count = i;
    return ret_commands;
}

char **split_and(char *command, int *count)
{
    char *temp;
    char **ret_commands = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        ret_commands[i] = malloc(1025 * sizeof(char));
    temp = strtok(command, "&");
    int i = 0;
    for (i = 0; temp != NULL; ++i)
    {
        strcpy(ret_commands[i], temp);
        temp = strtok(NULL, "&");
    }
    *count = i;
    return ret_commands;
}

char **split_words(char *command, int *count)
{
    char *temp;
    char **ret_commands = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        ret_commands[i] = malloc(1025 * sizeof(char));
    temp = strtok(command, " \t\r\n\a");
    int i;
    for (i = 0; temp != NULL; i++)
    {
        strcpy(ret_commands[i], temp);
        temp = strtok(NULL, "  \t\r\n\a");
    }
    *count = i;
    return ret_commands;
}

char **split_pipes(char *command, int *count)
{
    char *temp;
    char **ret_lines = malloc(64 * sizeof(char *));
    for (int i = 0; i < 64; i++)
        ret_lines[i] = malloc(1025 * sizeof(char));
    temp = strtok(command, "|");
    int i = 0;
    for (i = 0; temp != NULL; ++i)
    {
        strcpy(ret_lines[i], temp);
        temp = strtok(NULL, "|");
    }
    *count = i;
    return ret_lines;
}

uid_t getuid_cover(void)
{
    return getuid();
    // this fucntion is always successful. Therefore no error handling is needed
}

void gethost_cover(char *hostname, int size)
{
    int error_handler = gethostname(hostname, size);
    if (error_handler == -1)
    {
        perror("");
    }
    return;
}
char *read_line(int *x)
{
    int tab = 0;
    char c;
    int count = 0;
    char *command = malloc(BUFFERSIZE);
    char *second = malloc(BUFFERSIZE);

    setbuf(stdout, NULL);
    enableRawMode();
    memset(command, '\0', 100);
    memset(second, '\0', 1000);
    int pt = 0;
    int sec = 0;

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {

            if (c == 10)
                break;
            else if (c == 27)
            {
                tab = 0;

                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                    printf("\rarrow key: %s", buf);
                }
            }
            else if (c == 127)
            { // backspace
                tab = 0;

                if (pt > 0)
                {
                    if (command[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }

                    command[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {

                count = 0; // TAB character
                // printf("tab detected\n");
                char *temp = malloc(BUFFERSIZE);
                char *call = malloc(BUFFERSIZE);
                // command[pt++] = c;
                strcpy(second, command);
                temp = strtok(second, " ");
                while (temp)
                {
                    count++;
                    strcpy(call, temp);
                    temp = strtok(NULL, " ");
                }
                char *new = malloc(BUFFERSIZE);
                if (count == 2)
                {
                    // printf("%s\n",call);
                    if (tab == 0)
                    {
                        new = tab_fill(call);
                        tab = 1;
                        strcat(command, new);
                        pt += strlen(new);
                        Print_Init(root, host_name, user_name);
                        printf("%s", command);
                    }
                }
            }
            else if (c == 4)
            {
                tab = 0;
                printf("\n");
                exit(0);
            }
            else
            {
                tab = 0;
                printf("%d\n", c);
            }
        }
        else
        {
            tab = 0;
            command[pt++] = c;

            printf("%c", c);
        }
    }
    // command[pt++] = '\n';
    disableRawMode();
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i] != '\n')
            *x = 1;
    }
    return command;
}
struct passwd *getUserInfo(uid_t id)
{
    struct passwd *info_pass = malloc(sizeof(struct passwd));
    errno = 0;
    info_pass = getpwuid(id);
    if (info_pass == NULL)
        perror("");
    return info_pass;
}

void Print_Init(char *root, char *hostname, char *username)
{
    char final_path[1025];
    pathUpdate(root, final_path);

    printf("\033[0;37m<");
    printf("\033[0;32m%s@%s", username, hostname);
    printf("\033[0;37m:");
    printf("\033[0;34m%s", final_path);
    printf("\033[0;37m>");
    fflush(stdout);
}

void pathUpdate(char *root, char *final_path)
{
    char cwd[1025];
    getcwd(cwd, 1025);
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
    strcpy(final_path, cwd);
}
