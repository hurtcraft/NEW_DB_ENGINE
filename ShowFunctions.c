#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "Utils.h"
#include <dirent.h>
#define DATABASES "DATABASES"
#define TABLES "TABLES"

int isShowStatement(char *str,ShowStatement *showStatement);
void showDatabases(Env *env);
void showTables(Env *env);
int executeShowStatement(ShowStatement *showStatement, Env *env);
static char *showStatementPattern="^SHOW\\s+(DATABASES|TABLES);";
static regex_t showStatementRegex;

int initShowStatementRegex(){
    regcomp(&showStatementRegex,showStatementPattern,REG_EXTENDED);
}
int treatShowStatement(char *str, Env* env){
    ShowStatement showStatement={0};
    if(isShowStatement(str,&showStatement)){
        executeShowStatement(&showStatement,env);
        return 1; 
    }
    
    return 0;
}
int isShowStatement(char *str,ShowStatement *showStatement){
    regmatch_t matches[4];
    int ret=regexec(&showStatementRegex,str,4,matches,0);
    int tmpLen;
    if(ret==0){
        regmatch_t objectToShow=matches[1];
        tmpLen=objectToShow.rm_eo-objectToShow.rm_so;
        strncpy(showStatement->objectToShow,str+objectToShow.rm_so,tmpLen);
        return 1;
    }
    return 0;
}
int executeShowStatement(ShowStatement *showStatement, Env *env){
    if(strcmp(showStatement->objectToShow,DATABASES)==0){
        showDatabases(env);
    }
    else if(strcmp(showStatement->objectToShow,TABLES)==0){
        showTables(env);
    }
}

void showDatabases(Env *env){
    
    struct dirent** lstFile=malloc(sizeof(struct dirent*)*SMALL_BUFFER); 
    listFileInFolder(lstFile,env->WORPLACE);

    for (size_t i = 0; lstFile[i]!=NULL; i++)
    {
        printf("%s\n",lstFile[i]->d_name);
    }
    
    freeTabOfArray((void**)lstFile);

}

void showTables(Env *env){
    char pathMetaData[TINY_BUFFER];
    char tempCurrentDB[TINY_BUFFER];
    strcpy(tempCurrentDB,env->currentDatabase);// pour pas travailler direct sur env.currentdb    
    strcpy(pathMetaData,strcat(tempCurrentDB,METADATA_TABLES_PATH)); 

    struct dirent **listFile=malloc(sizeof(struct dirent*)*SMALL_BUFFER);
    listFileInFolder(listFile,pathMetaData);
    printf("%s\n",pathMetaData);
    for(size_t i=0;listFile[i]!=NULL;i++){
        printf("x %s \n",listFile[i]->d_name);
        // if(listFile[i]->d_name)// ICIIIIIIII
    }
    
    pathMetaData[0]='\0';

    freeTabOfArray((void**)listFile);
    printf("liberation mem\n");
}


char**getTabInfo(char *pathToTable){
    FILE *file;
    char str[SMALL_BUFFER]; 
    char **array=malloc(sizeof(char*)*MAX_TABLE_ATTRIBUTES);

    file = fopen(pathToTable, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return NULL;
    }

    while(fgets(str, SMALL_BUFFER, file) != NULL) {
        printf("%s \n", str);
    }

    fclose(file); 

    
    freeTabOfArray((void**)array);//a retire
}


