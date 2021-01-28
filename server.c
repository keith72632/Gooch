#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
#define FILE_NAME "index.html"
#define SIZE 1024

void sendFile(int new_sock){
    int n;
    FILE *fp;
    int file_size;
    char *data[202];

    fp = fopen(FILE_NAME, "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    if(fp == NULL)
        printf("[x]fp is null[x]\n");
    while(!feof(fp)){
        n = fread(data, sizeof(unsigned char), file_size, fp); 
//        printf("this is the html %s\n", data);
        if(n < 1){
            break;
        }
            
    }
    if(send(new_sock, data, sizeof(data), 0) < 0){
        printf("[x]Sending of html file failed[x]\n");
        exit(1);
    }
    fclose(fp);
    printf("[+]File sent. %d bytes\n", file_size);
}

int createSocket(){
    int temp = -1;
    temp = socket(AF_INET, SOCK_STREAM, 0);
    printf("[+]Socket created...\n");
    return temp;
}

int bindSocket(int server_sock){
    int temp = -1;
    struct sockaddr_in remote;
    memset(&remote, '\0', sizeof(remote)); 
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    temp = bind(server_sock, (struct sockaddr *)&remote, sizeof(remote));
    return temp;
}

int main() {
    int server_sock;
    char *lines = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    server_sock = createSocket();
   if(server_sock < 0){
      printf("[x]Socket Creation Failed[x]");
     return 1;
   } 

   if(bindSocket(server_sock) < 0){
       printf("[+]socket failed to bind[+]\n");
       return 1;
   }
   printf("[+]Socket successfully bound\n");

   printf("%s", lines);
   printf("[+]Listening on port %d...\n", PORT);
   printf("%s\n", lines);
   listen(server_sock, 1);
   
    while(1){
        int new_sock;
        struct sockaddr_in client;
        memset(&client, '\0', sizeof(client));
        int clientLen = sizeof(client);

        if((new_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&clientLen)) < 0){
            printf("client could not connect\n");
            exit(1);
        }
       printf("%s", lines);
       printf("[+]Connection made with %s\n", inet_ntoa(client.sin_addr));
       printf("%s\n", lines); 

       printf("%s", lines);
       sendFile(new_sock);
       printf("%s\n", lines);

    }
    return 0;
}
