#ifndef BF_HELPER
#define BF_HELPER
#include "common.h"
#include "bloomfilter.hpp"

bloom_filter init_bf(std::string _dataType, int _windowSize, float _errorRate);
#endif