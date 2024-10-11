#ifndef __UTILS__
#define __UTILS__
#include <regex.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "Entity.h"


char** split(char *str, char *delimiter);
void freeTabOfArray(void **tabOfArray);
void systemError(char *msg);
void systemMessage(char *msg);
int init(Env *env);
char *getCurrentDatabaseName(Env *env);

//StringUtils
void resetString(char *str);
char *join(char **str);
char* subString( char *source, int start, int end);
//FileUtils
struct dirent **listFileInFolder(struct dirent **direntList, char *pathFolder);
int createFolder(char *path);
int createFile(char* path);
int writeFile(FILE *f,char *data);
int fileExist(char *path);
int dirExist(char *path);
//RegexUtils
void initRegex();


//annexe
char *formatToCSV(char *buffer,char *data,int endFlag);
#endif