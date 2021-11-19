#ifndef TCP_SERVER
#define TCP_SERVER

#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <error.h>
#include "msg_controller/request_parser.hpp"

#ifndef MAXLINE
#define MAXLINE 4096
#endif

class TcpServer {
  private:
    int listenfd_;

  public:
    TcpServer();

    void BindAndListen(const char *host, const char *serv, socklen_t *addrlenp);
    // void Listen(const char *port);
    void AcceptByFork(void (*func)(int));
    void AcceptBySelect();
};

void ShowReceive(int sockfd);
#endif
