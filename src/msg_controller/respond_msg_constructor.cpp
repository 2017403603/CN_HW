#include "msg_controller/respond_msg_constructor.hpp"
#include <sstream>
#include <fstream>
#include "cgi_controller/php_cgi_controller.hpp"
#include <glog/logging.h>

RespondMsg RespondMsgConstructor::GetRespondMsg(RequestParser *request_parser) {
    std::string url = request_parser->GetUrl();
    if (!UrlExists(root_dir_, request_parser->GetUrl())) {
        LOG(WARNING) << root_dir_ << request_parser->GetUrl() << " is not exists";
        RespondMsg respond_msg = RespondMsg();
        // Todo set 404
        return respond_msg;
    }
    // if(url.end)
    std::string file_ext = GetFileExt(url);
    if (file_ext == "html" || file_ext == "htm"){
		std::fstream fs;
		fs.open(root_dir_ + url);
		if(fs)
			LOG(WARNING)<< "open file '" << root_dir_ + url << "failed";
		std::stringstream ss;
		ss << fs.rdbuf();
		std::string content = ss.str();
        return GetStaticPage(content);
	}
    if (file_ext == "php") {
        PhpCgiController php_cgi = PhpCgiController(root_dir_);
        std::string php_cgi_return = php_cgi.GetPHP(request_parser);
        int pos = php_cgi_return.find("\r\n\r\n");
        if (pos == -1)
            return RespondMsg();
        RespondMsg respond_msg = RespondMsg();
        respond_msg.SetBodyContent(php_cgi_return.substr(pos + 4));
        respond_msg.SetStatusCode(200);
        respond_msg.SetStatusCodeDesctiption("OK");
        return respond_msg;
    }
    return RespondMsg();
}
RespondMsg RespondMsgConstructor::GetStaticPage(std::string path) {
    RespondMsg respond_msg = RespondMsg();
    respond_msg.SetBodyContent(path);
    respond_msg.SetStatusCode(200);
    respond_msg.SetStatusCodeDesctiption("OK");
    return respond_msg;
}

std::string RespondMsgConstructor::GetFileExt(std::string filename) {
    int pos = filename.rfind(".");
    if (pos == -1)
        // Todo: Exception handle
        return "";
    return filename.substr(pos + 1);
}
bool RespondMsgConstructor::UrlExists(std::string root_dir, std::string url) {
    std::fstream fs;
    std::string path = root_dir + url;
    fs.open(path, std::ios::in);
    bool fileExists;
    fileExists = fs.good();
    fs.close();
    return fileExists;
}

RespondMsgConstructor::RespondMsgConstructor() {
    // todo: load conf file
    root_dir_ = "/home/jefung/repos/http_server_cpp/www";
    index_vector_.push_back("index.html");
    index_vector_.push_back("index.htm");
}
