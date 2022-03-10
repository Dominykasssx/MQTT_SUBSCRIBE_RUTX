#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "logger.h"
sqlite3 *db;

int flag = 0;
char *globalProgramName;

int callback(void *a_param, int argc, char **argv, char **column){
    for (int i=1; i< argc; i++)
    printf("%s\t", argv[i]);
    printf("\n");
    flag++;
    return 0;
}

int openDatabase(char *programName)
{
    int rc;
    char *zErrMsg = 0;

     if (programName != NULL){
        globalProgramName = programName;
    }
    else{
        return 1;
    }

    rc = sqlite3_open("/var/log/mqtt_subscriber.db", &db);

    if( rc ){
		sqlite3_close(db);
		return 1;
	}

    char *sql = (char*) malloc(sizeof(char) * 150);
        sprintf (sql,"CREATE TABLE IF NOT EXISTS Message (Id INTEGER PRIMARY KEY, Topic TEXT, Payload TEXT, Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);");

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    free(sql);
    if( rc ){
		sqlite3_close(db);
		return 1;
	}
    else{
    return 0;
    }
}

int closeDatabase(){
    return sqlite3_close(db);
}

int sqlQuery(char *message){
    
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_exec(db, message, callback, 0, &zErrMsg);
    free(message);
   
    if( rc != SQLITE_OK ){
        return 1;
        sqlite3_free(zErrMsg);
    } else {
        if(flag ==0){
            return 1;
        }
    }
    return 0;
}


int printLog(char *topic){    
    
    char *sql = (char*) malloc(sizeof(char) * 200);
    if (topic == NULL){
        sprintf (sql,"SELECT * FROM Message");
    }
    else{
        sprintf (sql,"SELECT * FROM Message WHERE Topic='%s';",topic);
    }
    int rc = sqlQuery(sql);
    if (rc == 1)
    {
        return 1;
    }
    return 0;
}


int saveLog(char *topic, char *payload){
     flag = -1;

    char *sql = (char*) malloc(sizeof(char) * 200);

    sprintf (sql,"INSERT INTO Message (Topic,Payload) VALUES ('%s','%s');",topic,payload);
    sqlQuery(sql);
    return 0;

}


