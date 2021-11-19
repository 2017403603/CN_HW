#include "msg_controller/request_parser.hpp"
#include <fstream>
#include <gtest/gtest.h>

TEST(RequestParser, SimpleGetTest) {

    std::ifstream ifle("/home/jefung/repos/http_server_cpp/simple_get.txt");
    std::string s;
    while (ifle)
        s.push_back(ifle.get());
    const char *str = s.c_str();
	const char *t = "test";
	// std::cout << strlen(t) << std::endl;
	// std::cout << t[4] << std::endl;
    // printf("%s", str);
    RequestParser request_parser = RequestParser();
	ASSERT_TRUE(request_parser.Parse(str,strlen(str)+1));
}

TEST(RequestParser, SimpleGetWithArgumentTest) {

    std::ifstream ifle("/home/jefung/repos/http_server_cpp/simple_get_with_arg.txt");
    std::string s;
    while (ifle)
        s.push_back(ifle.get());
    const char *str = s.c_str();
    // printf("%s", str);
    RequestParser request_parser = RequestParser();
	ASSERT_TRUE(request_parser.Parse(str,strlen(str)+1));
}


TEST(RequestParser, SimplePost) {

    std::ifstream ifle("/home/jefung/repos/http_server_cpp/simple_post.txt");
    std::string s;
    while (ifle)
        s.push_back(ifle.get());
    const char *str = s.c_str();
    // printf("%s", str);
    RequestParser request_parser = RequestParser();
	ASSERT_TRUE(request_parser.Parse(str,strlen(str)+1));
}


TEST(RequestParser, SimplePostWithArgumentTest) {

    std::ifstream ifle("/home/jefung/repos/http_server_cpp/simple_post_with_arg.txt");
    std::string s;
    while (ifle)
        s.push_back(ifle.get());
    const char *str = s.c_str();
    // printf("%s", str);
    RequestParser request_parser = RequestParser();
	ASSERT_TRUE(request_parser.Parse(str,strlen(str)+1));
	// request_parser.Print();
}
