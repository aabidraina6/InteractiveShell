#include "globals.h"

extern char host_name[65], root[1025], user_name[1025];


void Handler_sig()
{
    printf("\n");
    Print_Init(root , user_name,host_name);
    
}