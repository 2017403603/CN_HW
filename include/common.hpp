#ifndef COMMON
#define COMMON
// #define IS_DAEMON 1

#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <syslog.h>
// define http long connection time
// #define KEEP
const int kHttpConnectionTimeout = 300;

// this macro is used in class to construct function
// that operate private property
#define GET(func_suffix, name)          \
    decltype(name) Get##func_suffix() { \
        return name;                    \
    }

#define SET(func_suffix, name)                    \
    void Set##func_suffix(decltype(name) value) { \
        name = value;                             \
    }

#define GETANDSET(func_suffix, name) \
    GET(func_suffix, name);          \
    SET(func_suffix, name);

class Color {
    std::map<std::string, std::string> _color_map;

  public:
    Color();
    std::string Get(std::string color_name);
    bool IsExists(std::string color_name);
    std::string GetColorEnd();
};

std::string ToLower(std::string str);

void Output(std::string priority, const char *fmt, ...);

std::string ToLower(std::string str);

void ErrorExit(const char *fmt, ...);

int daemon_init(const char *program_name, int facility);

std::string FormatToString(const char *fmt, ...);

const char *IntToCStr(int number);
std::string GetFileExtension(const std::string &FileName);
#endif
