#include "pipe_networking.h"

char * SERVER_PIPE = "WKP";
/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo(SERVER_PIPE,0666);
  printf("Server:Creation\n");
  int client_server = open(SERVER_PIPE,O_RDONLY);

  char pipe_name[HANDSHAKE_BUFFER_SIZE];
  read(client_server, pipe_name, HANDSHAKE_BUFFER_SIZE);
  printf("Server:Name\n");

  remove(SERVER_PIPE);
  printf("Server: Remove pipe\n");

  int server_client = open(pipe_name,O_WRONLY);
  write(server_client,ACK,HANDSHAKE_BUFFER_SIZE);
  printf("Server: %s \n",ACK);

  char acknowledgement[HANDSHAKE_BUFFER_SIZE];
  read(client_server, acknowledgement, HANDSHAKE_BUFFER_SIZE);
  printf("Server: Recieved %s\n", acknowledgement);

  *to_client = server_client;
  return client_server;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("client_pipe",0666);
  printf("Client: Client creation\n");

  int client_server = open(SERVER_PIPE,O_WRONLY);
  char pipe_name[HANDSHAKE_BUFFER_SIZE] = "client_pipe";
  write(client_server, pipe_name, HANDSHAKE_BUFFER_SIZE);
  printf("Client: Name sent\n");

  int server_client = open("client_pipe",O_RDONLY);
  char acknowledgement[HANDSHAKE_BUFFER_SIZE];
  read(server_client, acknowledgement, HANDSHAKE_BUFFER_SIZE);
  printf("Client: Recieved %s\n", acknowledgement);

  remove("client_pipe");
  printf("Client: Removed client pipen");

  write(client_server, acknowledgement, HANDSHAKE_BUFFER_SIZE);
  printf("Client: Check confirmed \n");

  *to_server = client_server;
  return server_client;
}
