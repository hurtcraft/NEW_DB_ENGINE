#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "Utils.h"


static char* useStatementPattern="^USE\\s+(\\w+);";
static regex_t useStatementRegex;
int isUseStatement(char *str,UseStatement *useStatement, Env *env);
int initUseStatementRegex(){
    regcomp(&useStatementRegex,useStatementPattern,REG_EXTENDED);
}
int treatUseStatement(char *str,Env *env){
    static UseStatement useStatement={0};
    if(!isUseStatement(str,&useStatement,env)){
        systemError("database not found\n");        
    }

}

int isUseStatement(char *str,UseStatement *useStatement, Env *env){
    regmatch_t matches[3];
    int ret=regexec(&useStatementRegex,str,3,matches,0);
    int tmpLen;
    char tmpChar[TINY_BUFFER]={0};
    if (ret==0){
        regmatch_t dbName=matches[1];
        tmpLen=dbName.rm_eo-dbName.rm_so;
        strncpy(tmpChar,env->WORPLACE,strlen(env->WORPLACE));
        strncat(tmpChar,str+dbName.rm_so,tmpLen);
        
        if(dirExist(tmpChar)){
            strncpy(env->currentDatabase,tmpChar,strlen(tmpChar));
            return 1;
        }
    }
    return 0;
}

int dbExist(char *path){
    return dirExist(path);
}




