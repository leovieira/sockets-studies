// gcc udp_broadcaster.c -o udp_broadcaster.exe -l ws2_32

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define ADDR "192.168.0.255"
#define PORT 8888

int main() {
  WSADATA wsa;
  SOCKET soc;
  struct sockaddr_in saddr;
  char msg[] = "You look lonely?";

  WSAStartup(MAKEWORD(2, 2), &wsa);

  soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  saddr.sin_family = AF_INET;
  saddr.sin_addr.S_un.S_addr = inet_addr(ADDR);
  saddr.sin_port = htons(PORT);

  while (1) {
    sendto(soc, msg, strlen(msg), 0, (struct sockaddr *)&saddr, sizeof(saddr));
    Sleep(1000);
  }

  closesocket(soc);
  WSACleanup();

  return 0;
}
