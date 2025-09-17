#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Buffer length
#define BUFFER_LENGTH 4096

// Server port
#define PORT 8080

int main(int argc, char *argv[]) {
  // File descriptors
  int server, client;
  // Socket structures
  struct sockaddr_in saddr, caddr;
  // Buffer
  char buffer[BUFFER_LENGTH];

  fprintf(stdout, "Starting server...\n");

  // Create the server socket
  server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (server == -1) {
    fprintf(stderr, "Can't create the server socket:");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Server socket created.\n");

  // Define the server socket properties
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(PORT);

  // Bind the server socket to a port
  if (bind(server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
    fprintf(stderr, "Socket bind error:");
    return EXIT_FAILURE;
  }

  // Wait connections from clients
  if (listen(server, 1) == -1) {
    fprintf(stderr, "Listen error:");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Listening on port %d.\n", PORT);

  socklen_t caddr_len = sizeof(caddr);
  ssize_t msg_len;

  // Accept connection
  client = accept(server, (struct sockaddr *) &caddr, &caddr_len);
  if (client == -1) {
    fprintf(stderr, "Accept error:");
    return EXIT_FAILURE;
  }

  // Copies the welcome message into buffer
  strcpy(buffer, "Hello, client!\n");

  // Sends welcome message to the client
  if (send(client, buffer, strlen(buffer), 0) == -1) {
    fprintf(stderr, "Failed to send welcome message:");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Connection established.\nWaiting for client messages...\n");

  do {
    // Clear the buffer
    memset(buffer, 0x0, BUFFER_LENGTH);
 
    // Receives message from the client
    msg_len = recv(client, buffer, BUFFER_LENGTH, 0);
    if (msg_len > 0) {
      // Replay
      send(client, buffer, msg_len, 0);

      buffer[msg_len - 1] = '\0';
      fprintf(stdout, "Client says: %s\n", buffer);
    }
  } while(strcmp(buffer, "bye"));

  close(client);
  close(server);

  fprintf(stdout, "Conection closed.\n");
  return EXIT_SUCCESS;
}
