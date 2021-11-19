#include "tcp_server.hpp"
#include "common.hpp"
#include "msg_controller/respond_msg.hpp"
#include "msg_controller/respond_msg_constructor.hpp"
#include <glog/logging.h>
#include <signal.h>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>

// add it to common.hpp?
void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        LOG(INFO) << "child process " << pid << " is terminated";
}

TcpServer::TcpServer() {
}

void TcpServer::BindAndListen(const char *host, const char *serv, socklen_t *addrlenp) {
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    // set the request message type
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // get list contain addrinfo
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        ErrorExit("tcp bind error for %s, %s: %s", host, serv, gai_strerror(n));
    ressave = res;

    do {
        listenfd = socket((int) res->ai_family, (int) res->ai_socktype, (int) res->ai_protocol);
        if (listenfd < 0)
            continue; // error, try next node

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        close(listenfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        ErrorExit("tcp_listen error for %s, %s, %s", host, serv);

    listen(listenfd, 1024);

    if (addrlenp)
        *addrlenp = res->ai_addrlen;

    freeaddrinfo(ressave);
    listenfd_ = listenfd;
}

void TcpServer::AcceptByFork(void (*func)(int)) {
    struct sockaddr_in client_addr, server_addr;
    int connfd;
    socklen_t client_len;

    // add child process exit signal handle
    signal(SIGCHLD, sig_chld);

    pid_t child_pid;
    int connection_number = 0;
    while (1) {
        client_len = sizeof(client_addr);
        if ((connfd = accept(listenfd_, (sockaddr *) &client_addr, &client_len)) < 0) {
            // EINTR:  Interrupted system call
            if (errno == EINTR)
                continue;
            else
                ErrorExit("accept error: %s", strerror(errno));
        }
        LOG(INFO) << "Accep cliend: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
        LOG(INFO) << "current connection number: " << ++connection_number;

        // fork to generate child process
        if ((child_pid = fork()) == 0) {
            close(listenfd_);
            func(connfd);
            LOG(INFO) << "It's going to close client[" << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port)
                      << "]";
            close(connfd);
        	LOG(INFO) << "current connection number: " << --connection_number;
            exit(0);
        }
        close(connfd);
    }
}

void TcpServer::AcceptBySelect() {
}

bool connect_flag = true;
void CloseConnect(int signo) {
    std::cout << "CloseConnect" << std::endl;
    signal(SIGALRM, SIG_IGN);
    connect_flag = false;
}

//Show receive message
void ShowReceive(int sockfd) {
    int i = 0;
    ssize_t n;
    char buf[MAXLINE];
    RespondMsgConstructor respond_msg_constructor;
    alarm(kHttpConnectionTimeout);

    struct itimerval new_value, old_value;
	int request_num = 0;
    while (connect_flag) {
        n = read(sockfd, buf, MAXLINE);
        if (n < 0 && errno == EINTR)
            continue;
        else if (n < 0)
            ErrorExit("ShowReceive Error");
        if (n > 0) {
            RequestParser request_parser;
            request_parser.Parse(buf, n);
            alarm(kHttpConnectionTimeout);
            RespondMsg respond_msg = respond_msg_constructor.GetRespondMsg(&request_parser);
            respond_msg.send(sockfd);
        }
    }
}
