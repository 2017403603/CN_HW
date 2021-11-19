#ifndef RESPOND_CONSTRUCTOR
#define RESPOND_CONSTRUCTOR
#include "common.hpp"
#include "iostream"
#include "sstream"

class RespondMsg {
  private:
    std::string msg_;
    double version_;
    int status_code_;
    std::string status_code_description_;
	
	// respond header must be need
    int content_length_;
    std::string content_type_;
	std::string connection_;

    std::map<std::string, std::string> header_map_;
    std::string body_content_str_;
	char *body_content_p_;

  public:
    RespondMsg();
    void send(int sockfd);
    void length();
    GETANDSET(Msg, msg_);
    GETANDSET(StatusCode, status_code_);
    GETANDSET(StatusCodeDesctiption, status_code_description_);
    GETANDSET(Version, version_);
    GETANDSET(ContentLength, content_length_);
    GETANDSET(ContentType, content_type_);
    GETANDSET(Connection, connection_);
	// void SetBodyContent(std::string body_content);
	void SetBodyContent(std::string content);
    // GETANDSET(BodyContent, body_content_);
};

#endif
