#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

// Buffer length
#define BUFFER_LENGTH 4096

// Server port
#define PORT 8080

void respond(int sockfd) {
  char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
  send(sockfd, response, strlen(response), 0);
}

void *conn_handler(void *param) {
  int sockfd = *((int*) param);
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);

  if (getpeername(sockfd, (struct sockaddr *) &peer_addr, &peer_addr_len) == -1) {
    fprintf(stderr, "Failed to get peer data.\n");
    pthread_exit(NULL);
  }

  fprintf(
    stdout,
    "Connection established with %s:%d.\n",
    inet_ntoa(peer_addr.sin_addr),
    ntohs(peer_addr.sin_port)
  );

  respond(sockfd);
  fprintf(
    stdout,
    "Sends the HTML to %s:%d.\n",
    inet_ntoa(peer_addr.sin_addr),
    ntohs(peer_addr.sin_port)
  );

  close(sockfd);
  fprintf(
    stdout,
    "Conection closed with %s:%d.\n",
    inet_ntoa(peer_addr.sin_addr),
    ntohs(peer_addr.sin_port)
  );

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  // File descriptors
  int server, client;
  // Socket structures
  struct sockaddr_in saddr, caddr;

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
  int *sockfd;

  // Accept connection
  while(client = accept(server, (struct sockaddr *) &caddr, &caddr_len)) {
    pthread_t conn_thread;
    sockfd = (int*) malloc(sizeof(int));
    if (sockfd == NULL) {
      fprintf(stderr, "Failed to allocate memory:");
      return EXIT_FAILURE;
    }
    *sockfd = client;

    if (pthread_create(&conn_thread, NULL, conn_handler, (void*) sockfd) != 0) {
      fprintf(stderr, "Could not create thread:");
      return EXIT_FAILURE;
    }
    pthread_detach(conn_thread);
  }

  if (client == -1) {
    fprintf(stderr, "Accept error:");
    return EXIT_FAILURE;
  }

  close(server);
  return EXIT_SUCCESS;
}
