#include <stdio.h>
#include <stdlib.h>
#include "ser_outside_xc/thirdParty/sqlite/sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("params.conf", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "UPDATE HTTP_PARAMS SET VIN = 'VCEC360BL00017544' ,TOKEN = 'VCEC360BL00017544544' ,TYPE=0 ,TERMINAL_NUMBER = '8001',VEHICLE_NUMBER = 280301,SIM = '134123489234',PASSWD = '63910938' ,AuxDomain = 'http://125.33.25.66' ,AuxPort = 28003 WHERE ID = 1;";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }



   sqlite3_close(db);
   return 0;
}
