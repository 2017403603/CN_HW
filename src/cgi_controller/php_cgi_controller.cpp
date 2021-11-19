#include "cgi_controller/php_cgi_controller.hpp"
#include "common.hpp"
#include "msg_controller/request_parser.hpp"
#include <glog/logging.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

PhpCgiController::PhpCgiController(std::string www_root) {
    www_root_ = www_root;
}
std::string PhpCgiController::GetPHP(RequestParser *rq) {
    setenv("TEST", "TEST_VALUE", 1);
    // setenv
    // const char *cmd = "ls -l ";
    // Todo: add setenv wrap to adapt to any type
    setenv("REQUEST_METHOD", rq->GetMethod().c_str(), 1);
    setenv("SCRIPT_FILENAME", (www_root_ + rq->GetUrl()).c_str(), 1);
    setenv("REDIRECT_STATUS", "true", 1);
    setenv("CONTENT_LENGTH", IntToCStr(rq->GetContentLength()), 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
    if (rq->ExistsCookie()) {
        std::string cookie = rq->GetHEADERS()["Cookie"];
        setenv("HTTP_COOKIE", cookie.c_str(), 1);
    }
    string cmd;
    if (rq->GetContentLength())
        cmd = "echo " + rq->GetBodyContent() + " | /usr/bin/php-cgi";
    else
        cmd = "/usr/bin/php-cgi";
    FILE *pipe;
    LOG(INFO) << cmd;
    // Output("info","%s",cmd.c_str());
    pipe = popen(cmd.c_str(), "r");
    if (pipe == NULL)
        cout << cmd << " execute error";
    char line[1024];
    std::string content;
    while (fgets(line, sizeof(line), pipe) != NULL) {
        content += line;
        // printf("%s", line);
    }
    pclose(pipe);
    return content;
}
