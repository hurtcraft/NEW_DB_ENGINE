
#include "Utils.h"
#include <string.h>
#include "CONSTANT.h"
#include <stdlib.h>
#include <stdio.h>
#include "Entity.h"
#include <windows.h>

#include <string.h>

char** split(char *str, char *delimiter) {
    char **buffer = malloc(BIG_BUFFER*sizeof(char*));
    
    char *cpy=strdup(str);
    char *strToken = strtok(cpy, delimiter);  // Token initial
    int nbToken = 0;

    while (strToken != NULL) {
        buffer[nbToken++] = strdup(strToken); 
        strToken = strtok(NULL, delimiter);   
    }

    buffer[nbToken] = NULL; 

    return buffer;
}

// char **split( char* str,char *delimiter){
//     static char *result[SMALL_BUFFER]={0};
//     char token[BIG_BUFFER]={0};

//     int indexToken=0;
//     int nbToken=0;
//     for (size_t i = 0; i < strlen(str); i++)
//     {
//         if(str[i]!=delimiter[0]){
//             token[indexToken]=str[i];
//             indexToken++;
//         }
//         else{
//             result[nbToken]=malloc(sizeof(char)*strlen(token)+1);
//             strcpy(result[nbToken],token);
//             printf("result %s\n",result[nbToken]);
//             memset(token,0,BIG_BUFFER);
//             nbToken++;
//             indexToken=0;
//         }
//     }
//     if(indexToken>0){
//         result[nbToken]=malloc(sizeof(char)*strlen(token));
//         strcpy(result[nbToken],token);
//         nbToken++;

//     }
//     result[nbToken]=NULL;
//     return result;
    

// }

void freeTabOfArray(void **tabOfArray)
{
    for (size_t i = 0; tabOfArray[i] != NULL; i++)
    {
        free(tabOfArray[i]);
    }
    free(tabOfArray);
}


void systemError(char *msg)
{
    fprintf(stderr, "[ERROR SYSTEM] %s\n", msg);
}
void systemMessage(char *msg)
{
    fprintf(stdout, "[SYSTEM] %s\n", msg);
}


int init(Env *env)
{
    initRegex();
    char SYSDIR[SMALL_BUFFER]={0};
    int dirSize = GetSystemDirectory(SYSDIR, SMALL_BUFFER);
    env->currentDatabase[0] = '\0';
    if (dirSize < 0 || dirSize > SMALL_BUFFER)
    {
        systemError("CRITIC FAILED AT INITIATING WORKING FOLDER");
        exit(1);
    }
    char *destination=subString( SYSDIR, 0, 2); //=>c:/
    strcpy(env->WORPLACE, strcat(destination, "SUPA_DATABASE_ENGINE\\"));
    strcpy(env->currentDatabase,UNKNOW_DB);
    createFolder(env->WORPLACE);
    return 0;
}
char *getCurrentDatabaseName(Env *env){
    if(strcmp(env->currentDatabase,UNKNOW_DB)==0){
        return UNKNOW_DB;
    }
    int start=strlen(env->WORPLACE);
    int end=strlen(env->currentDatabase); 
    printf("%d %d \n",start,end);
    return subString(env->currentDatabase,start,end);
}


char *formatToCSV(char *buffer,char *data,int endFlag){
    //check taille buffer
    strcat(buffer,data);
    if(!endFlag){
        strcat(buffer,";");
    }
}