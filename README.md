# C++ Server in a Unix environment

A concurrent-thread Server is up and waiting for any client to connect and send a SQL request. When a client connected to the server, the server will create a slave-thread to serve the connected client and thus to free the server to wait for any other incoming client. The slave-thread is then to receive a SQL statement from the client and call the sqlite3 database in the background to process the SQL transaction. The outcome of the SQL transaction is then to be sent back to the client. 
