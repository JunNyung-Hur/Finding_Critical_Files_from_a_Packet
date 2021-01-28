#ifndef COMMON_H
#define COMMON_H
#define NOMINMAX

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <set>
#include <ctime>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <openssl/md5.h>

#include <curl/curl.h>

#include <pcap.h>

#define RAPIDJSON_NOMEMBERITERATORCLASS
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#ifdef SAM_DEF
#define EXT
#else
#define EXT extern
#endif
EXT std::string INDEX_DIR;
EXT std::string DIRECTORY_NAME;
EXT std::string BLOOMFILTER_DIR;
EXT std::string PCAP_DIR;

EXT unsigned int WINDOW_SIZE;
EXT float BF_ERROR_RATE;

EXT std::string ES_ADDR;
EXT unsigned int ES_SHARDS;
EXT unsigned int ES_REPLICAS;
EXT unsigned int ES_INDEX_INTERVAL;

#endif 

