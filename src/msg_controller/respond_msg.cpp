#include "msg_controller/respond_msg.hpp"
#include <glog/logging.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include <fstream>
extern int errno;

RespondMsg::RespondMsg() {
    msg_ = "";
    version_ = 1.1;
    status_code_ = 500;
    content_length_ = 0;
    content_type_ = "text/html";
    body_content_str_ = "";
    body_content_p_ = NULL;
    connection_ = "Keep-Alive";
    // connection_ = "close";
}

void RespondMsg::send(int sockfd) {
    std::stringstream respond_stream;
    respond_stream << "HTTP/" << version_ << ' ' << status_code_ << ' ' << status_code_description_ << "\r\n";
    respond_stream << "Content-type: " << content_type_ << "\r\n";
    respond_stream << "Content-Length: " << content_length_ << "\r\n";
    respond_stream << "Connection: " << connection_ << "\r\n";
    respond_stream << "\r\n";
    respond_stream << body_content_str_;
    std::string respond_msg = respond_stream.str();
	LOG(INFO) << respond_msg;
    write(sockfd, respond_msg.c_str(), respond_msg.length() + 1);
}

void RespondMsg::length() {
}

void RespondMsg::SetBodyContent(std::string content) {
	body_content_str_=content;
	content_length_ = content.length();
}
