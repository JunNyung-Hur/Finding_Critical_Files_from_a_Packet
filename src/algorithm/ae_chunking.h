#include "common.h"

#ifndef AE_CHUNKING_H
#define AE_CHUNKING_H
std::vector<std::string> ae_chunking(const unsigned char* byteSeq, const unsigned int bytesSize, const unsigned int windowSize);
#endif