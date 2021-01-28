#ifndef BF_HELPER_H
#define BF_HELPER_H
#include "common.h"
#include "ae_chunking.h"
#include "utils.h"
#include "bloomfilter.hpp"

//std::string INDEX_DIR;
//std::string BLOOMFILTER_DIR;

bloom_filter init_bf(std::string _dirName, int _windowSize, float _errorRate);
#endif