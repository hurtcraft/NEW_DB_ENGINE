#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "CONSTANT.h"
#include <dirent.h>
#include <windows.h>
struct dirent **listFileInFolder(struct dirent **direntList, char *pathFolder)
{
    struct dirent *entree;
    DIR *dir = opendir(pathFolder);
    if (dir == NULL)
    {
        return NULL;
    }
    int count = 0;
    while ((entree = readdir(dir)) != NULL)
    {
        direntList[count] = malloc(sizeof(struct dirent));

        memcpy(direntList[count], entree, sizeof(struct dirent));

        ++count;
    }
    direntList[count] = NULL;
    closedir(dir);
    return direntList;
}

int createFolder(char *path)
{
    if (CreateDirectory(path, NULL))
    {
        return 1;
    }
    return 0;
}



int createFile(char* path){
    FILE *f=fopen(path,"w");

    if(f==NULL){
        return 1;
    }
    fclose(f);
    return 0;

}

int writeFile(FILE *f,char *data){
    fprintf(f,data);
    fclose(f);
    return 0;
}