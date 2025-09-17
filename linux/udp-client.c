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

// Server address
#define ADDR "127.0.0.1"

// Server port
#define PORT 8080

int main(int argc, char *argv[]) {
  // File descriptor
  int sockfd;
  // Socket structure
  struct sockaddr_in saddr;
  // Buffer
  char buffer[BUFFER_LENGTH];

  fprintf(stdout, "Starting client...\n");

  // Create the socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sockfd == -1) {
    fprintf(stderr, "Can't create the socket:");
    return EXIT_FAILURE;
  }

  // Define the socket properties
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(ADDR);
  saddr.sin_port = htons(PORT);

  socklen_t saddr_len = sizeof(saddr);
  ssize_t msg_len;

  do {
    // Clear the buffer
    memset(buffer, 0x0, BUFFER_LENGTH);

    fprintf(stdout, "Say something to the server: ");
    fgets(buffer, BUFFER_LENGTH, stdin);
    fflush(stdin);

    // Sends message to the server
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &saddr, sizeof(saddr));

    // Receives message from the server
    msg_len = recvfrom(sockfd, buffer, BUFFER_LENGTH, 0, (struct sockaddr *) &saddr, &saddr_len);
    if (msg_len > 0) {
      buffer[msg_len] = '\0';
      fprintf(stdout, "Server says: %s\n", buffer);
    }
  } while (strcmp(buffer, "bye"));

  close (sockfd);
  return EXIT_SUCCESS;
}
