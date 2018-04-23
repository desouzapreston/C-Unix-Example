//Preston deSouza
//CS 3377.0W2
//4-22-18

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sqlite3.h>
#include <string.h>

pthread_mutex_t lock;

struct serverParm{
    int connectionDesc;
};

char* concat(const char* s1, const char* s2){
    char* result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static int callback(void *data, int argc, char *argv[], char *azColName[]){
    char** returnMsg;
    returnMsg = (char**)data;
    int i = 0;
	for(i = 0; i < argc; i++){
        *returnMsg = concat(*returnMsg, azColName[i]);
        *returnMsg = concat(*returnMsg, " = ");
        *returnMsg = concat(*returnMsg, argv[i] ? argv[i] : NULL);
        *returnMsg = concat(*returnMsg, "\n");
    }
    *returnMsg = concat(*returnMsg, "\n");
    return 0;
}

//Server thread deals with message processing from clients
void *serverThread(void *parmPtr){

    //Load in sql databse
    sqlite3 *db;
    if(sqlite3_open("employee",&db)){
        fprintf(stderr, "SERVER: Can't open database %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else{
        fprintf(stdout, "Database has been opened: \n");
    }


#define PARMPTR ((struct serverParm *) parmPtr)
    int recievedMsgLen, rc=0;
    char messageBuf[1024],*zErrMsg = 0;

    printf("Connection successful:  \n", PARMPTR->connectionDesc);
    //Check if connection descriptor is valid
    if(PARMPTR->connectionDesc < 0){
        printf("Accept failed\n");
        return(0);
    }
    
    //Recieve messages from client
    while((recievedMsgLen=read(PARMPTR->connectionDesc,messageBuf,sizeof(messageBuf)-1)) > 0){
        char* data = "";
        //null terminate the c-string
        recievedMsgLen[messageBuf] = '\0';

        pthread_mutex_lock(&lock);
        //Send recieved message to sqlite3_exec
        rc = sqlite3_exec(db, messageBuf, callback, (void*)&data, &zErrMsg);
        if(rc != SQLITE_OK){
            fprintf(stderr, "SERVER: SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else{
            fprintf(stdout, "Check Success #1\n");
        }
        pthread_mutex_unlock(&lock);
        
        if(write(PARMPTR->connectionDesc, data, 1024) < 0){
            perror("SERVER: write error");
            return 0;
        }
        
        if(strcmp(data,"") != 0)
            free(data);

        fprintf(stdout, "\n-idle state-\n");
    }

    //Avoid descriptor and memory leaks
    close(PARMPTR->connectionDesc);
    free(PARMPTR);
    return(0);

}

int main(int argc, char* argv[]){
    int listenDesc, PORTNUMBER;
    struct sockaddr_in myAddr;
    struct serverParm *parmPtr;
    int connectionDesc;
    pthread_t threadID;
    
    if(argc != 2){
        perror("TCPServer <Portnumber>");
        exit(1);
    }

    PORTNUMBER = atoi(argv[1]);

    //Force the process to eventually terminate if things go wrong
    alarm(360);
    
    //Create mutex for synchronization
    if(pthread_mutex_init(&lock,NULL) != 0){
        printf("\nSERVER: mutex init failed\n");

        return 1;
    }

    //create socket for reading
    if((listenDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("open error on socket");
        exit(1);
    }

    //creating the "name" of the socket
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(PORTNUMBER);

    if(bind(listenDesc, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0){
        perror("bind error");
        exit(1);
    }
    
    //Start accepting connections
    //Allow up to 5 connections to be queued
    listen(listenDesc,5);
    
    while(1){
        //wait for client connection
        connectionDesc = accept(listenDesc, NULL,NULL);
        
        //create thread to handle client
        parmPtr = (struct serverParm *)malloc(sizeof(struct serverParm));
        parmPtr-> connectionDesc = connectionDesc;
        if(pthread_create(&threadID, NULL, serverThread, (void*)parmPtr) != 0){
            perror("Thread create error");
            close(connectionDesc);
            close(listenDesc);
            exit(1);
        }

        printf("Ready for new client\n");
    }

    pthread_mutex_destroy(&lock);
}
