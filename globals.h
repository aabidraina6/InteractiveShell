#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <ctype.h>


#define BUFFERSIZE 1024

char **split_lines(char *command, int *count);
char **split_and(char *command, int *count);
char **split_words(char *command, int *count);
char **split_pipes(char *command, int *count);
uid_t getuid_cover(void);
void gethost_cover(char *hostname, int size);
char *read_line(int *x);
struct passwd *getUserInfo(uid_t id);
void Print_Init(char *root, char *hostname, char *username);
void pathUpdate(char *root, char *final_path);
int cd_util(char **command, char *root);
int disc_util(char *root, int flag_d, int flag_f, 
char *target, char *file_name, char *toprint, int count);
int echo_util(char **message,int n);
void save_history(char *root, char *input);
void history_print(char *root);
int ls_util(int flag, char *name, char **dir_names, int n);
int lsl_util(int flag, char *name, char **dir_names, int n);
void ls_caller(char **words, char *root, int n);
int pinfo_util(char **words, char *root, int n);
void Update(char *cwd, char *root, char *final_path);
int pwd_util();
int count_blocks(int flag, char *name, char **dir_names, int count);
void sigChild_intr();
int runCommand(int background, char **args, int pos);
int addBg(int pid, char *command);
void removeBg(int pid);
int prev_path(char *path, char **store);
void autofill(char **words, char *root, int count);
char *tab_fill (char *curr );
int jobs_util(int *bgpid , char **bgCommand ,  char **args);
int sig_util(int jobid, int signo);
int bg_util(int jobid);
void Handler_sig();


