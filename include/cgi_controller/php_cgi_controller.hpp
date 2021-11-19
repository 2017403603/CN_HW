#ifndef PHP_CGI_CONTROLLER
#define PHP_CGI_CONTROLLER
#include <string>
#include "common.hpp"
#include "msg_controller/request_parser.hpp"
class PhpCgiController{
	private:
		std::string www_root_;
	public:
		PhpCgiController(std::string www_root);
		std::string GetPHP(RequestParser *rq);
};

#endif
