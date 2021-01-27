#ifndef UTILS
#define UTILS
#include "common.h"

void print_chunk_arr(std::vector<std::vector<unsigned char>> _chunk_arr);

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
std::string get_md5(std::string _data);

#include "utils.hpp"
#endif