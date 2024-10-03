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
int treatShowStatement(char *str, ShowStatement *showStatement, Env* env){
    if(isShowStatement(str,showStatement)){
        executeShowStatement(showStatement,env);
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
    
    struct dirent** lstFile=malloc(sizeof(struct dirent*)*100); 
    listFileInFolder(lstFile,env->WORPLACE);

    for (size_t i = 0; lstFile[i]!=NULL; i++)
    {
        printf("%s\n",lstFile[i]->d_name);
    }
    
    freeTabOfArray((void**)lstFile);

}

void showTables(Env *env){
    char pathMetaData[TINY_BUFFER];
    printf("c1 %s \n",env->currentDatabase);
    strcpy(pathMetaData,strcat(env->currentDatabase,METADATA_TABLES_PATH)); 
    printf("c2 %s \n",env->currentDatabase);

    printf(" final path %s\n",pathMetaData);
    pathMetaData[0]='\0';
}