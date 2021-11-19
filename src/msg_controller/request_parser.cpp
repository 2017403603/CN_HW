#include "msg_controller/request_parser.hpp"
#define CHECK_EOF()        \
    if (buf == buf_end_) { \
        return false;      \
    }

#define EXPECT_CHAR_NO_CHECK(ch) \
    if (*buf++ != ch) {          \
        state_code_ = -1;        \
        return false;            \
    }
#define EXPECT_CHAR(ch) \
    CHECK_EOF();        \
    EXPECT_CHAR_NO_CHECK(ch);

RequestParser::RequestParser() {
    buf_end_ = NULL;
    state_code_ = true;
    msg_ = "";
}
bool RequestParser::Parse(const char *buf, int buf_len) {
    buf_end_ = buf + (buf_len - 1);
    return ParseRequestLine(buf);
}

bool RequestParser::ParseRequestLine(const char *buf) {
    msg_ = "parser request line error";
    CHECK_EOF();
    return ParseRequestMethod(buf);
}

bool RequestParser::ParseRequestMethod(const char *buf) {
    msg_ = "parser request method error";
    char c;
    while (1) {
        CHECK_EOF();
        c = *buf;
        if (c == ' ')
            return ParseUrl(++buf);
        else {
            if (c >= 65 && c <= 91)
                method_ += c;
            else {
                return false;
            }
            buf++;
        }
    }
}

bool RequestParser::ParseUrl(const char *buf) {
    msg_ = "parser url error";
    char c;
    while (1) {
        CHECK_EOF();
        c = *buf;
        if (c == ' ')
            return ParseHttpVersion(++buf);
        else {
            // Todo: need to do filter about url
            if (c == '?') {
                return ParseUrlVarible(++buf);
            }
            url_ += c;
        }
        buf++;
    }
}
bool RequestParser::ParseUrlVarible(const char *buf) {
    msg_ = "parse url variable error";
    std::string key = "";
    std::string value = "";
    char c;
    while (1) {
        key = "";
        while (1) {
            CHECK_EOF();
            c = *buf;
            if (c == ' ') {
                if (key != "")
                    url_var_map_[key] = "";
                return ParseHttpVersion(++buf);
            } else {
                if (c == '=') {
                    buf++;
                    break;
                }
                key += c;
                buf++;
            }
        }

        value = "";
        while (1) {
            CHECK_EOF();
            c = *buf;
            if (c == ' ') {
                url_var_map_[key] = value;
                return ParseHttpVersion(++buf);
            } else {
                if (c == '&') {
                    url_var_map_[key] = value;
                    ++buf;
                    break;
                } else {
                    value += c;
                    buf++;
                }
            }
        }
    }
}

bool RequestParser::ParseHttpVersion(const char *buf) {
    // return true;
    msg_ = "parser http version error";
    EXPECT_CHAR('H');
    EXPECT_CHAR('T');

    if (buf == buf_end_) {
        return false;
    }
    EXPECT_CHAR('T');
    // if (buf == buf_end_) {
    // }
    EXPECT_CHAR('P');
    EXPECT_CHAR('/');
    EXPECT_CHAR('1');
    EXPECT_CHAR('.');
    if (*buf++ == '1') {
        version_ = 1.1;
    } else
        return false;

    EXPECT_CHAR(BACKSLASHR);
    EXPECT_CHAR(BACKSLASHN);
    return ParseHeader(buf);
}

bool RequestParser::ParseHeader(const char *buf) {
    msg_ = "parse header error";
    std::string header_key;
    std::string header_value;
    while (1) {
        header_key = "";
        header_value = "";
        CHECK_EOF();
        if (*buf == BACKSLASHR) {
            EXPECT_CHAR(BACKSLASHR);
            EXPECT_CHAR(BACKSLASHN);
            break;
        }
        while (*buf != ':') {
            header_key += *buf;
            buf++;
            CHECK_EOF();
        }
        buf++;
        EXPECT_CHAR(' ');
        while (*buf != BACKSLASHR) {
            header_value += *buf;
            buf++;
            CHECK_EOF();
        }
        EXPECT_CHAR(BACKSLASHR);
        EXPECT_CHAR(BACKSLASHN);
        header_map_[header_key] = header_value;
    }
    return ParseBody(buf);
}

bool RequestParser::ParseBody(const char *buf) {
    do {
        body_content_ += *(buf);
        buf++;
    } while (buf != buf_end_);
    return true;
}

void RequestParser::Print() {
    std::cout << "method:" << method_ << std::endl;
    std::cout << "url: " << url_ << std::endl;
    std::cout << "version: " << version_ << std::endl;

    std::map<std::string, std::string>::iterator iter;
    for (iter = header_map_.begin(); iter != header_map_.end(); iter++)
        std::cout << iter->first << " : " << iter->second << std::endl;

    std::cout << "url variable: " << std::endl;
    for (iter = url_var_map_.begin(); iter != url_var_map_.end(); iter++)
        std::cout << iter->first << " : " << iter->second << std::endl;

    std::cout << "body: " << std::endl
              << body_content_ << std::endl;
}
std::string RequestParser::GetMsg() {
    return msg_;
}
int RequestParser::GetContentLength() {
    std::map<std::string, std::string>::iterator iter;
    iter = header_map_.find("Content-Length");
    if (iter != header_map_.end()) {
        return atoi(iter->second.c_str());
    }
    return 0;
}
void RequestParser::SetHeaders(std::string k,std::string v){
	header_map_[k] = v;
}
bool RequestParser::ExistsCookie(){
	if(header_map_.find("Cookie") == header_map_.end())
		return false;
	else return true;
}
