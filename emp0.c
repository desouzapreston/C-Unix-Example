// g++ text.c -l sqlite3

#include <stdio.h>
#include <sqlite3.h> 

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("employee.db", &db);

   if( rc ){
      fprintf(stderr, "Can't open employee database: %s\n", sqlite3_errmsg(db));
   }else{
      fprintf(stderr, "Opened employee database successfully\n");
   }
   sqlite3_close(db);
}
