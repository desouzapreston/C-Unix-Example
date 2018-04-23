
Assignment5: a5Server1 a5Client1 a5Client2 a5Client3 emp0 emp1 emp2 emp3 emp4 emp5 emp6 employee.db

a5Server1: a5Server1.c
	gcc a5Server1.c -o a5Server1 -lpthread -lsqlite3

a5Client1: a5Client1.c
	gcc a5Client1.c -o a5Client1 

a5Client2: a5Client2.c
	gcc a5Client2.c -o a5Client2
	
a5Client3: a5Client3.c
	gcc a5Client3.c -o a5Client3

emp0: emp0.c
	g++ emp1.c -o emp0 -l sqlite3

emp1: emp1.c
	g++ emp1.c -o emp1 -l sqlite3

emp2: emp2.c
	g++ emp2.c -o emp2 -l sqlite3

emp3: emp3.c
	g++ emp3.c -o emp3 -l sqlite3

emp4: emp4.c
	g++ emp4.c -o emp4 -l sqlite3

emp5: emp5.c
	g++ emp5.c -o emp5 -l sqlite3

emp6: emp6.c
	g++ emp6.c -o emp6 -l sqlite3

employee.db:
	./emp0
	./emp1
	./emp2
	./emp3
	./emp4
	./emp5
	./emp6

clean:
	rm -rf a5Server1 a5Client1 a5Client2 a5Client3 emp0 emp1 emp2 emp3 emp4 emp5 emp6 employee.db

runserver:
	./a5Server 10296

runc1:
	./a5Client1 127.0.0.1 10296

runc2:
	./a5Client2 127.0.0.1 10296

runc3:
	./a5Client3 127.0.0.1 10296

