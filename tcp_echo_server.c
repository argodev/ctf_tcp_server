/*
 *  A CTF example.  Has read, write, execute, allocation, infoleak primitives
 *  in a simple TCP socket server, where each connection is handled in a different
 *  thread.
 *
 *  To compile:
 *    gcc tcp_echo_server.c -lpthread -o server
 *
 *  Code based on:
 *    https://gist.github.com/oleksiiBobko/43d33b3c25c03bcc9b2b
*/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
 
struct command {
    size_t command;
    size_t arg1;
    size_t arg2;
};


//the thread function
void *connection_handler(void *);
void handle_command(struct command *);
typedef void func(size_t);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[sizeof(struct command)];
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , sizeof(client_message), MSG_WAITALL)) > 0 )
    {
        if (read_size != sizeof(client_message)) {
                break;
        }
		
        handle_command((struct command *)client_message);

		//Send the message back to client
        write(sock , client_message , sizeof(client_message));
		
		//clear the message buffer
		memset(client_message, 0, sizeof(client_message));
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;
} 

void handle_command(struct command *cmd) {
    void *ptr;
    func *f;
    switch (cmd->command) {
        case 0x61:  //'a'
            ptr = realloc((void *)(cmd->arg1), cmd->arg2);    //This will malloc, realloc, and free; if called correctly
            cmd->arg2 = (size_t)ptr;
            break;
        case 0x72:  //'r'
            cmd->arg2 = *(size_t *)(cmd->arg1);
            break;
        case 0x77:  //'w'
            *(size_t *)(cmd->arg1) = cmd->arg2;
            break;
        case 0x78:  //'x'
            f = (func *)(cmd->arg1);
            f(cmd->arg2);
            break;
        default:
            memset(cmd, 0, sizeof(struct command));
            break;
    }
    return;
}

