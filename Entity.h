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
    char type[NANO_BUFFER];
    int size;
    int isPrimaryKey;
    int isNotNull;
    int isAutoIncrement;
    int isUnique;
    char foreignKey[MEDIUM_BUFFER];
    int faileAtInitFlag;
}Field;


typedef struct CreateStatement
{
    char objectToCreate[NANO_BUFFER];
    char objectName[TINY_BUFFER];
    char args[BIG_BUFFER];
}CreateStatement;



typedef struct ShowStatement{
    char objectToShow[TINY_BUFFER];
    char objectName[TINY_BUFFER];
}ShowStatement;

typedef struct UseStatement{
    char databaseName[TINY_BUFFER];
}UseStatement;





#endif