#ifndef REQUEST_PARSER
#define REQUEST_PARSER

#include <common.hpp>
#include <iostream>
#include <map>
#include <string>
// define '\r' return
#define BACKSLASHR '\x0d'
// define '\n' newline
#define BACKSLASHN '\x0a'

class RequestParser {
  private:
    const char *buf_end_;
    bool state_code_;
    std::string msg_;
    std::string method_;
    std::string url_;
	std::map<std::string,std::string> url_var_map_;
    double version_;
    std::map<std::string, std::string> header_map_;
    std::string body_content_;

  public:
    RequestParser();
    bool Parse(const char *buf, int buf_len);
    bool ParseRequestLine(const char *buf);
    bool ParseRequestMethod(const char *buf);
    bool ParseUrl(const char *buf);
	bool ParseUrlVarible(const char *buf);
    bool ParseHttpVersion(const char *buf);
    bool ParseHeader(const char *buf);
    bool ParseBody(const char *buf);
    // bool AfterParse();
    void Print();
    std::string GetMsg();
	int GetContentLength();
	void SetHeaders(std::string k,std::string v);
    GETANDSET(Url, url_);
    GETANDSET(Method, method_);
	GETANDSET(BodyContent,body_content_);
	GETANDSET(HEADERS,header_map_);
	bool ExistsCookie();
};

#endif
