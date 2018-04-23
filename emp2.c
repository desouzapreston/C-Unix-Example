#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

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
   int rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("employee.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "INSERT INTO EMPLOYEE (ID,NAME,AGE,ADDRESS,SALARY) " \
         "VALUES (1, 'John1', 32, 'Texas', 20000.00 ); " 	\
         "INSERT INTO EMPLOYEE (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'John2', 25, 'Texas', 15000.00 ); "  	\
         "INSERT INTO EMPLOYEE (ID,NAME,AGE,ADDRESS,SALARY)"   \
         "VALUES (3, 'John3', 23, 'Texas', 20000.00 );" 	\
         "INSERT INTO EMPLOYEE (ID,NAME,AGE,ADDRESS,SALARY)"   \
         "VALUES (4, 'John4', 25, 'Texas', 65000.00 );"   	\
         "INSERT INTO EMPLOYEE (ID,NAME,AGE,ADDRESS,SALARY)"   \
         "VALUES (5, 'John5', 50, 'Texas', 90000.00 );";


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
