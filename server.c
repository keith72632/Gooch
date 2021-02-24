#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#define PORT 8080
#define FILE_INDEX "index.html"
#define FILE_INFO "info.html"
#define SIZE 11000

void sendFile(int new_sock, FILE *fp){
    int n;
    int file_size;
    char *data[SIZE];

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    if(fp == NULL)
        printf("[x]fp is null[x]\n");
    while(!feof(fp)){
        n = fread(data, sizeof(unsigned char), SIZE, fp); 
        if(n < 1){
            break;
        }
            
    }
    if(send(new_sock, data, n, 0) < 0){
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
    time_t raw_time;
    struct tm * time_info;
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

        //time
        time(&raw_time);
        time_info = localtime(&raw_time);
        if((new_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&clientLen)) < 0){
            printf("client could not connect\n");
            exit(1);
        }
       printf("%s", lines);
       printf("[+]Connection made with %s\n", inet_ntoa(client.sin_addr));
       printf ("[+]time and date: %s", asctime (time_info) );
       printf("%s\n", lines); 

       FILE *fp1;
       FILE *fp2;
       fp1 = fopen(FILE_INDEX, "r");
       fp2 = fopen(FILE_INFO, "r");

       printf("%s", lines);
       sendFile(new_sock, fp1);
       printf("%s\n", lines);

    }
    return 0;
}
