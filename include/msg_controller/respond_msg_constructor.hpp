#ifndef RESPOND_MSG_CONSTRUCTOR
#define RESPOND_MSG_CONSTRUCTOR
#include "common.hpp"
#include "fstream"
#include "msg_controller/request_parser.hpp"
#include "msg_controller/respond_msg.hpp"
#include "vector"

class RespondMsgConstructor {
  private:
    std::vector<std::string> index_vector_;
    std::string root_dir_;

  public:
    RespondMsgConstructor();
    RespondMsg GetRespondMsg(RequestParser *request_parser);
    bool UrlExists(std::string root_dir, std::string url);
    RespondMsg GetStaticPage(std::string path);
    std::string GetFileExt(std::string filename);
};

#endif
