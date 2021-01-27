#ifndef COMMON
#define COMMON
#define NOMINMAX

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include <ctime>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <openssl/md5.h>

#include <curl/curl.h>

#include "pcap.h"

#define RAPIDJSON_NOMEMBERITERATORCLASS
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static const std::string RAW_DIR = "C:\\Users\\hurjn\\Documents\\실험데이터\\infocom\\raw\\";
static const std::string BF_DIR = "C:\\Users\\hurjn\\Documents\\실험데이터\\infocom\\bloom_filter\\c++\\";
static const std::string PCAP_DIR = "C:\\Users\\hurjn\\Documents\\실험데이터\\infocom\\pcap\\";
static const std::string DATA_TYPE = "wiki";
static const std::string PTC_TYPE = "ftp";
static const std::string ES_ADDR = "localhost:9200";
static const unsigned int ES_SHARDS = 5;
static const unsigned int ES_REPLICAS = 0;
static const unsigned int ES_INDEX_INTERVAL = 1;


#include "ae_chunking.h"
#include "utils.h"
#endif 

