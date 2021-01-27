#ifndef AE_CHUNKING
#define AE_CHUNKING
#include "common.h"
std::vector<std::string> ae_chunking(const unsigned char* byteSeq, const unsigned int bytesSize, const unsigned int windowSize);
#endif