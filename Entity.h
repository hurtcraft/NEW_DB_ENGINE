#ifndef ENV
#define ENV
#include "CONSTANT.h"
typedef struct Env
{
    char WORPLACE[SMALL_BUFFER];
    char currentDatabase[MEDIUM_BUFFER];
}Env;

typedef struct Field{
    // NAME;TYPE;SIZE;PK?;NN?;AI;UNQ;FK?;

    char name[MEDIUM_BUFFER];
    int type;
    int size;
    int isPrimaryKey;
    int isNotNull;
    int isAutoIncrement;
    int isUnique;
    char foreignKey[MEDIUM_BUFFER];
}Field;


typedef struct CreateStatement
{
    char objectToCreate[NANO_BUFFER];
    char objectName[TINY_BUFFER];
    char args[BIG_BUFFER];
}CreateStatement;

typedef struct ShowStatement{
    char objectToShow[TINY_BUFFER];
}ShowStatement;




#endif