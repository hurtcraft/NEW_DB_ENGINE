#include <stdio.h>
#include "Utils.h"
#include <string.h>
#include "Entity.h"
#include "Functions.h"

#include <dirent.h>
int main(){
    Env env={0};
    init(&env);

    treatCreateStatement("CREATE TABLE matable (AGE INT PRIMARY KEY,PRENOM VARCHAR(256) PRIMARY KEY NOT NULL FOREIGN KEY REFERENCES PERSONNES(ABC));",&env);
    // treatCreateStatement("CREATE DATABASE dbTest2;",&env);
    // treatCreateStatement("CREATE DATABASE dbTest3;",&env);
    // treatCreateStatement("CREATE DATABASE dbTest4;",&env);
    // treatCreateStatement("CREATE DATABASE dbTest5;",&env);
    

    //strcpy(env.currentDatabase,"C:\\SUPA_DATABASE_ENGINE\\dbTest2");
    //faire structure table
    //treatShowStatement("SHOW TABLES;",&env);


    //treatUseStatement("USE dbTest3;",&env);
    // printf("la db name : %s\n ",getCurrentDatabaseName(&env));
    
    printf("[FIN]");
    return 0;
}


// int main(){


//     char str[] = "bonjour tlm ceci est un test";
//     char str2[]= "lalalallala bvabbabab ccccc eeee fff ggg aaa kk lle ";
//     char **test=split(str2," ");
//     for(size_t i =0;test[i]!=NULL;i++){
//         printf("test %s\n",test[i]);
//     }
//     test=split(str," ");

//     for(size_t i =0;test[i]!=NULL;i++){
//         printf("test %s\n",test[i]);
//     }


//     freeTabOfArray((void**)test);
//     return 0;
// }