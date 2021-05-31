#include "strutils.h"
#include <string.h>

void get_pretty_cwd(char *dest, char * to_be_replaced)
{
    const char * rep = "~";
    char *p = strstr(dest, to_be_replaced);
    if(p)   // If the substring was found
    {
        char buf[1024] = {'\0'};

        if(dest == p)
        {
            strcpy(buf, rep);
            strcat(buf, p+strlen(to_be_replaced));
        }
        else
        {
            strncpy(buf,dest, strlen(dest) - strlen(p));
            strcat(buf,rep);
            strcat(buf, p + strlen(to_be_replaced));
        }

        memset(dest,'\0', strlen(dest));
        strcpy(dest, buf);
    }
}

void trim_semic(char * str){
    char lastch = str[strlen(str)-1];
    if(lastch == ';')
        str[strlen(str)-1] = '\0';
}
