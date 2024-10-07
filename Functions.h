#ifndef FUNCTIONS
#define FUNCTIONS
#include "Entity.h"
void initRegex();
// int getMatch(char*originStr,regmatch_t *match,char* buffer);

int initCreateStatementRegex();
int initShowStatementRegex();


int treatCreateStatement(char *str,Env *env);
int treatShowStatement(char *str, Env* env);
int treatUseStatement(char *str,Env *env);
int treatDropStatement(char *str);
int treatAlterStatement(char *str);
int treatSelectStatement(char *str);
int treatInsertStatement(char *str);
int treatUpdateStatement(char *str);
int treatDeleteStatement(char *str);



#endif