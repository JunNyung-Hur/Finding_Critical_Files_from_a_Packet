#ifndef UTILS_H
#define UTILS_H

#include "common.h"


void print_chunk_arr(std::vector<std::vector<unsigned char>> _chunkArr);

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
std::string get_md5(std::string _data);
void parse_config();

#include "utils.hpp"
#endif