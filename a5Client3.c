
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

//Max text length 
#define MAXLINE 4096

int main(int argc, char** argv){
    unsigned int SERV_PORT;
    int sockfd;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE];

    //In case something goes wrong the process will end in 120s
    alarm(120);

    //Check for correct number of arguments
    if(argc != 3){
        perror("Unable to receive <Server IP> <Server Ports>");
        exit(1);
    }
    SERV_PORT = atoi(argv[2]);
    
    //Create a socket for the client
    //if the socket fd < 0 than an error occured while creating the socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ERROR: Failed to create socket");
        exit(2);
    }

    //Creation of the socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); //converts to big-endian order

    //Connection of the client to the socket
    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))<0){
        perror("Problem in connection to the server");
        exit(3);
    }
    char sql1Command[] = "DELETE from EMPLOYEE where ID = 5;";
    while(send(sockfd,sql1Command,strlen(sql1Command), 0)){
            
        if(recv(sockfd,recvline,MAXLINE,0) == 0 ){
            perror("The server terminated prematurely");
            exit(4);
        }

        printf("\n%s\n", "Cleared entry from C3: ");
        sleep(2);
        
        char sql2Command[] = "INSERT INTO COMPANY (ID, AGE, ADDRESS, SALARY) \nVALUES(5, 'John5', 50, 'Texas', 90000.00)";

        if(send(sockfd,sql2Command,strlen(sql2Command), 0) == -1)
            exit(0);

        if(recv(sockfd, recvline,MAXLINE,0) == 0){
            perror("CLIENT3: The server terminated prematurely");
            exit(4);
        }

        printf("\n%s\n", "Restored entry from C3: ");
        sleep(2);

        char sql1Command[] = "UPDATE EMPLOYEE set NAME = '*** UPDATED ***' where ID = 1;";
    }

    exit(0);
    }
