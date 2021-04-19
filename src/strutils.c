#include "strutils.h"
#include <string.h>

void get_pretty_cwd(char *src, char * str)
{
    const char * rep = "~";
    char *p = strstr(src, str);
    if(p)   // If the substring was found
    {
        char buf[1024] = {'\0'};

        if(src == p)
        {
            strcpy(buf, rep);
            strcat(buf, p+strlen(str));
        }
        else
        {
            strncpy(buf,src, strlen(src) - strlen(p));
            strcat(buf,rep);
            strcat(buf, p + strlen(str));
        }

        memset(src,'\0', strlen(src));
        strcpy(src, buf);
    }
}
