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
   sql = "CREATE TABLE PARAMS("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "VIN            CHAR(17) NOT NULL," \
         "TOKEN          CHAR(20) NOT NULL," \
         "MapV           CHAR(23) NOT NULL," \
         "SoftV          CHAR(10) NOT NULL," \
         "HardV          CHAR(10) NOT NULL," \
         "Cycle          INT     NOT NULL," \
         "AuxDomain         CHAR(30) NOT NULL," \
         "AuxPort        INT     NOT NULL," \
         "Domain         CHAR(30) NOT NULL," \
         "Port           INT     NOT NULL );" ;

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }

   /* Create SQL statement */
   sql = "INSERT INTO PARAMS(ID,VIN,TOKEN, MapV,SoftV,HardV,Cycle,Domain,Port,AuxDomain,AuxPort)"  \
         //"VALUES (1, 'TL850430CK8570015','TL850430CK8570015015',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 1, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'TL850430CK8570016','TL850430CK8570016016',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 1, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'VCEC360BL00017544','VCEC360BL00017544544',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 1, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'CLW009LFCFC000869','CLW009LFCFC000869869',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 0, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         "VALUES (1, 'EQMA1K18623008658','EQMA1K18623008658658',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 0, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'EQPE067492LJT9001','EQPE067492LJT9001001',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 0, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'EQPE0674LJZ389002','EQPE0674LJZ389002002',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 0, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";
         //"VALUES (1, 'EQPE0674928XC9003','EQPE0674928XC9003003',  '12123.23456.55332.32156', '1.10.10.10', '1.10.10.10', 0, 'qas.was.eqfleetcmder.com', 28001 , 'http://125.33.25.66:28003' , 28003 ); ";

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
