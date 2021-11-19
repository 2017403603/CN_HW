#include <glog/logging.h>
#include <string>
#include <iostream>
#include <syslog.h>
#include <unistd.h>
// #include "common.hpp"
#include "tcp_server.hpp"

int main(int argc, char *argv[]) {

    // Start google log system
    FLAGS_log_dir = "/home/jefung/repos/http_server_cpp/log";
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true; // Set log color
    FLAGS_logbufsecs = 0;          // Set log output speed(s)
    FLAGS_max_log_size = 1024;     // Set max log file size

    TcpServer tcp_server = TcpServer();
    tcp_server.BindAndListen(NULL, "9877", NULL);
    tcp_server.AcceptByFork(ShowReceive);

	// close google log system for memory not overflow
    google::ShutdownGoogleLogging();
}
