#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "CONSTANT.h"
#include <string.h>
void resetString(char *str)
{
    int len = strlen(str);
    memset(str, 0, len);
    str[0] = '\0';
}

char *join(char **str)
{
    static char result[1024] = {0};
    result[0] = '\0';

    for (size_t i = 0; str[i] != NULL; i++)
    {
        strcat(result, str[i]);
        if (str[i + 1] != NULL)
        {
            strcat(result, " ");
        }
    }

    return result;
}

char* subString(char *source, int start, int end)
{
    static char destination[BIG_BUFFER]={0};
    size_t i;
    for (i = 0; i < end - start + 1; ++i)
    {
        destination[i] = source[i + start];
    }
    destination[i] = '\0';
    return destination;
    // printf("destination %s\n",destination);
    // printf("strlen %d i %d\n",strlen(destination),i);
    // printf("last %d\n",(unsigned char)destination[strlen(destination)+1]);
};
