#include "common.hpp"
#include "sstream"

const char* IntToCStr(int number){
	std::stringstream ss;
    ss << number;
    const char *c_str = ss.str( ).c_str( );
	return c_str;
}
