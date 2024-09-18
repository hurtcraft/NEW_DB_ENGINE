#ifndef FUNCTIONS
#define FUNCTIONS
#include "Entity.h"
void initRegex();
// int getMatch(char*originStr,regmatch_t *match,char* buffer);

int initCreateStatementRegex();


int treatCreateStatement(char *str, CreateStatement *createStatement,Env *env);
int treatShowStatement(char *str);
int treatDropStatement(char *str);
int treatUseStatement(char *str);
int treatAlterStatement(char *str);
int treatSelectStatement(char *str);
int treatInsertStatement(char *str);
int treatUpdateStatement(char *str);
int treatDeleteStatement(char *str);



#endif