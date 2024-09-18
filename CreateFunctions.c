#include "Functions.h"
#include "Entity.h"
#include <regex.h>
#include "Utils.h"
#include <string.h>


int isCreateStatement(char *str,CreateStatement *createStatement);
void executeCreateStatement(CreateStatement *createStatement,Env *env);
void createDatabase(CreateStatement *createStatement,Env *env);
void createTable(CreateStatement *createStatement,Env *env);
void parseTableBody(CreateStatement *createStatement);
Field parseField(char* field);
int getTypeSize(char* type);

static char* createStatemntPattern="(CREATE) (DATABASE|TABLE) (\\w+)\\s*(\\(([^}]*)\\))?\\s*\\;$";
static char* validFieldRegexPattern="(\\w+)\\s+(INT|VARCHAR\\((d+)\\))";
static regex_t createStatementRegex;
static regex_t validFieldRegex;
static char* NOT_NUll_ARGS="NOT NULL";
static char* AUTO_INC_ARGS="AUTO_INCREMENT";
static char* PRIMARY_KEY_ARGS="PRIMARY KEY";
static char* UNIQUE_ARGS="UNIQUE";
static char* FOREIGN_KEY_ARGS="FOREIGN KEY REFERENCES";


int initCreateStatementRegex(){
    regcomp(&createStatementRegex,createStatemntPattern,REG_EXTENDED);
    regcomp(&validFieldRegex,validFieldRegexPattern,REG_EXTENDED);
}
int treatCreateStatement(char *str, CreateStatement *createStatement,Env *env){
   if(isCreateStatement(str,createStatement)){
        executeCreateStatement(createStatement,env);
   }
    return 0;
}

int isCreateStatement(char *str,CreateStatement *createStatement){
    regmatch_t matches[5];

    int ret=regexec(&createStatementRegex,str,5,matches,0);
    if(ret==0){
        int tmpLen;
        regmatch_t objectToCreate=matches[2];
        regmatch_t objectName=matches[3];
        regmatch_t args=matches[4];

        tmpLen=objectToCreate.rm_eo-objectToCreate.rm_so;
        strncpy(createStatement->objectToCreate ,str+objectToCreate.rm_so,tmpLen);//TABLE or DATABASE
        tmpLen=objectName.rm_eo-objectName.rm_so;
        strncpy(createStatement->objectName,str+objectName.rm_so,tmpLen);
        if(args.rm_so!=-1){
            tmpLen=args.rm_eo-args.rm_so;
            strncpy(createStatement->args,str+args.rm_so,tmpLen);
        }
        return 1;
    }

    return 0;
} 


void executeCreateStatement(CreateStatement *createStatement,Env *env){
    if(strcmp(createStatement->objectToCreate,"DATABASE")==0){
        createDatabase(createStatement,env);
    }
    else if(strcmp(createStatement->objectToCreate,"TABLE")==0){
        createTable(createStatement,env);
    }

}



void createDatabase(CreateStatement *createStatement,Env *env){
    char *path=strcat(env->WORPLACE,createStatement->objectName);
    char *metaData=strcat(path,METADATA_TABLES_PATH);
    
    createFolder(path);
    createFolder(metaData);
}

void createTable(CreateStatement *createStatement,Env *env){
    parseTableBody(createStatement);
}   


void parseTableBody(CreateStatement *createStatement){
    char *args=createStatement->args;
    char *trimedArgs=subString(createStatement->args,1,strlen(args)-2);
    char **splitBuffer=split(trimedArgs,",");
    char *copy;
    Field f;
    for (size_t i = 0; splitBuffer[i]!=NULL; i++)
    {
        printf("splitBuffer %s\n",splitBuffer[i]);
        split(strdup(splitBuffer[i])," ");
        // parseField(copy);
    }
    

    freeTabOfArray((void**)splitBuffer);
}


Field parseField(char* field){
    printf("[%s]\n",field);
    // structure d'un champs : 
    // NAME;TYPE;SIZE;PK?;NN?;AI;UNQ;FK?;
    char**splitBuffer=split(field," ");
    Field f;
    // if(nbElt<2){
    //     f.type=FAILED_AT_INIT;
    //     return f;
    // }
    char *nom=splitBuffer[0];
    char *type=splitBuffer[1];
    int size =getTypeSize(type);

    int argsStartIndex=strlen(nom)+strlen(type)+2;// 2 pour les 2 espaces
    int argsEndIndex=strlen(field);


    char *args=subString(field,argsStartIndex,argsEndIndex);
    // printf("args %s\n",args);

    char subArgs[NANO_BUFFER]={0};
    if(strstr(args,PRIMARY_KEY_ARGS)!=NULL){
        printf("est PK\n");
    };
    if(strstr(args,UNIQUE_ARGS)!=NULL){
        printf("est UNQ\n");
    }
    if(strstr(args,AUTO_INC_ARGS)!=NULL){
        printf("est AUTO_INC\n");
    }
    if(strstr(args,NOT_NUll_ARGS)!=NULL){
        printf("est NN\n");
    }
    if(strstr(args,FOREIGN_KEY_ARGS)!=NULL) {
        printf("est FK\n");
    }
    // for(size_t i =0;splitBuffer[i]!=NULL;i++){
        
    //     printf("%d %s \n",i,splitBuffer[i]);
    // }
    printf("finu\n");
    return f;
}

int getTypeSize(char* type){
    static char* INT_STR="INT";
    static char* VARCHAR_STR="VARCHAR(";
    if(strcmp(type,INT_STR)==0){
        return 32;
    }
    else if(strstr(type,VARCHAR_STR)!=NULL){
        char *tmp=subString(type,strlen(VARCHAR_STR),strlen(type)-2);
        
        return atoi(tmp);    
    }
    return -1;
}

