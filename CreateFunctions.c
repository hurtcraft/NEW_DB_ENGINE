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
char* getForeignKey(char *args);
void resetCreateStatement(CreateStatement *createStatement);

static char* NOT_NUll_ARGS="NOT NULL";
static char* AUTO_INC_ARGS="AUTO_INCREMENT";
static char* PRIMARY_KEY_ARGS="PRIMARY KEY";
static char* UNIQUE_ARGS="UNIQUE";
static char* FOREIGN_KEY_ARGS="FOREIGN KEY REFERENCES";

static char* createStatemntPattern="(CREATE) (DATABASE|TABLE) (\\w+)\\s*(\\(([^}]*)\\))?\\s*\\;$";
static char* validFieldRegexPattern="(\\w+)\\s+(INT|VARCHAR\\((d+)\\))";
static char* foreignKeyRegexPattern="^.*FOREIGN KEY REFERENCES\\s+(\\w+\\(\\w+\\))";
static regex_t createStatementRegex;
static regex_t validFieldRegex;
static regex_t foreignKeyRegex;



int initCreateStatementRegex(){
    regcomp(&createStatementRegex,createStatemntPattern,REG_EXTENDED);
    regcomp(&validFieldRegex,validFieldRegexPattern,REG_EXTENDED);
    regcomp(&foreignKeyRegex,foreignKeyRegexPattern,REG_EXTENDED);
}
int treatCreateStatement(char *str,Env *env){
    CreateStatement createStatement={0};
    resetCreateStatement(&createStatement);
   if(isCreateStatement(str,&createStatement)){
        executeCreateStatement(&createStatement,env);
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
    char worplace[TINY_BUFFER];
    strcpy(worplace,env->WORPLACE);
    char *path=strcat(worplace, createStatement->objectName);
    createFolder(path);
    char *metaData=strcat(path,METADATA_TABLES_PATH);
    createFolder(metaData);

    worplace[0]='\0';
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
        // NAME;TYPE;SIZE;PK?;NN?;AI;UNQ;FK?;

    for (size_t i = 0; splitBuffer[i]!=NULL; i++)
    {
        // split(strdup(splitBuffer[i])," ");
        f=parseField(splitBuffer[i]);
        if(f.faileAtInitFlag==FAILED_AT_INIT){
            printf("pb parsefield \n");
        }
        printf("fielkd %s %s %d %d %d %d %d %s \n ",f.name,f.type,f.size,f.isPrimaryKey,f.isNotNull,f.isAutoIncrement,f.isUnique,f.foreignKey);
    }
    

    freeTabOfArray((void**)splitBuffer);
}


Field parseField(char* field){
    printf("[%s]\n",field);
    // structure d'un champs : 
    // NAME;TYPE;SIZE;PK?;NN?;AI;UNQ;FK?;
    char**splitBuffer=split(field," ");

    Field f={0};
    int nbElt=0;
    for (size_t i = 0; splitBuffer[i] !=NULL; i++)
    {
        ++nbElt;
    }
    if(nbElt<2){
        f.faileAtInitFlag=FAILED_AT_INIT;
        return f;
    }

    // char *nom=splitBuffer[0];
    // char *type=splitBuffer[1];

    char nom[NANO_BUFFER]={0};
    char type[NANO_BUFFER]={0};

    strncpy(f.name,splitBuffer[0],strlen(splitBuffer[0]));
    strncpy(f.type,splitBuffer[1],strlen(splitBuffer[1]));

    int size =getTypeSize(f.type);
    if(size==-1){
        f.faileAtInitFlag=FAILED_AT_INIT;
        return f;
    }
    f.size=size;
    int argsStartIndex=strlen(nom)+strlen(type)+2;// 2 pour les 2 espaces
    int argsEndIndex=strlen(field);

    char *args=subString(field,argsStartIndex,argsEndIndex);

    char subArgs[NANO_BUFFER]={0};
    if(strstr(args,PRIMARY_KEY_ARGS)!=NULL){
        printf("est PK\n");
        f.isPrimaryKey=1;
    };
    if(strstr(args,UNIQUE_ARGS)!=NULL){
        printf("est UNQ\n");
        f.isUnique=1;
    }
    if(strstr(args,AUTO_INC_ARGS)!=NULL){
        printf("est AUTO_INC\n");
        f.isAutoIncrement=1;
    }
    if(strstr(args,NOT_NUll_ARGS)!=NULL){
        printf("est NN\n");
        f.isNotNull=1;
    }
    if(strstr(args,FOREIGN_KEY_ARGS)!=NULL) {
        printf("est FK\n");
        strcpy(f.foreignKey,getForeignKey(args));
    }
    freeTabOfArray((void**)splitBuffer);
    return f;
}
char* getForeignKey(char *args){
    regmatch_t matches[2];
    int ret =regexec(&foreignKeyRegex,args,2,matches,0);
    static char result[NANO_BUFFER]={0};
    if(ret==0){
        printf("FK trouve \n");
        regmatch_t fk=matches[1];
        int len=fk.rm_eo-fk.rm_so;
        
        strncpy(result,args+fk.rm_so,len);
        printf("fk %s\n",result);
        
        return result;
    }
    return NULL;
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

void resetCreateStatement(CreateStatement *createStatement){
    memset(createStatement->args,0,BIG_BUFFER);
    memset(createStatement->objectName,0,TINY_BUFFER);
    memset(createStatement->objectToCreate,0,NANO_BUFFER);


}

    