#include "common.hpp"
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <syslog.h>
Color::Color() {
    _color_map["white"] = "\033[37m";
    _color_map["bule"] = "\033[34m";
    _color_map["red"] = "\033[31m";
    _color_map["green"] = "\033[32m";
    _color_map["purple"] = "\033[35m";
}

std::string Color::Get(std::string color_name) {
    std::map<std::string, std::string>::iterator iter;
    iter = _color_map.find(color_name);
    if (iter != _color_map.end())
        return iter->second;
    else
        return "";
}

bool Color::IsExists(std::string color_name) {
    std::map<std::string, std::string>::iterator iter;
    iter = _color_map.find(color_name);
    if (iter != _color_map.end())
        return true;
    else
        return false;
}

std::string Color::GetColorEnd() {
    return "\033[0m";
}

void OutPutWithColor(std::string priority, std::string msg) {
    std::string show_color;
    Color color;
    if (priority == "notice")
        show_color = color.Get("bule");
    if (priority == "info")
        show_color = color.Get("white");
    if (priority == "warn")
        show_color = color.Get("red");
    if (priority == "error")
        show_color = color.Get("red");
    if (!show_color.empty()) {
        std::cout << show_color << msg << color.GetColorEnd() << std::endl;
    } else
        std::cout << msg << std::endl;
}
void Output(std::string priority, const char *fmt, ...) {

	// translate variable arguments into msg string according to format
    std::string msg;
    va_list ap;
    va_start(ap, fmt);
    // msg = FormatToString(fmt, ap);
	char pchar[4096];
	vsnprintf(pchar,4096,fmt,ap);
	msg = pchar;
    va_end(ap);

	// according piority (like err, info...) to handle msg
    std::string priority_lower = ToLower(priority);
#ifdef IS_DAEMON
    std::cout << "define IS_DAEMON" << std::endl;
    if (priority_lower == "info")
        syslog(LOG_INFO, "%s", msg.c_char());
    if (priority_lower == "notice")
        syslog(LOG_NOTICE, "%s", msg.c_char());
    if (priority_lower == "warn")
        syslog(LOG_WARNING, "%s", msg.char());
    return;
#endif
    OutPutWithColor(priority, msg);
}

void ErrorExit(const char *fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	Output("error",fmt,ap);
	va_end(ap);
	exit(1);
}

std::string ToLower(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32;
    }
    return str;
}

// how size it's fit?
#ifndef MAXFORMATLENTH
#define MAXFORMATLENTH 4096
#endif
std::string FormatToString(const char *fmt, ...) {
    char str_char[MAXFORMATLENTH];
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vsnprintf(str_char, MAXFORMATLENTH, fmt, ap);
	// std::cout << "vsnprintf: " << str_char << std::endl;
    std::string str = str_char;
    return str;
}
