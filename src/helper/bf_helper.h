#ifndef BF_HELPER_H
#define BF_HELPER_H
#include "common.h"
#include "ae_chunking.h"
#include "utils.h"
#include "bloomfilter.hpp"

bloom_filter init_bf(std::string _indexName, int _windowSize, float _errorRate);
#endif
