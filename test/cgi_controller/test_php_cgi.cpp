
#include "common.hpp"
#include <gtest/gtest.h>
#include "cgi_controller/php_cgi_controller.hpp"
#include "msg_controller/request_parser.hpp"

TEST(php_cig, test_post_var) {
	PhpCgiController php_cgi("/home/jefung/repos/http_server_cpp/test/test_data");	
	RequestParser rq;
	rq.SetUrl("/test_post.php");
	rq.SetMethod("POST");
	rq.SetBodyContent("gtest=gtest_value");
	rq.SetHeaders("Content-Length","17");
	std::string content = php_cgi.GetPHP(&rq);
	EXPECT_NE(content.find("gtest_value"),std::string::npos);
}

TEST(php_cig, test_cookie) {
	PhpCgiController php_cgi("/home/jefung/repos/http_server_cpp/test/test_data");	
	RequestParser rq;
	rq.SetUrl("/test_cookie.php");
	rq.SetMethod("POST");
	rq.SetBodyContent("gtest=gtest_value");
	rq.SetHeaders("Content-Length","17");
	rq.SetHeaders("Cookie","gtest_cookie=this_is_a_test_cookie_value");
	std::string content = php_cgi.GetPHP(&rq);
	EXPECT_NE(content.find("this_is_a_test_cookie_value"),std::string::npos);
}
