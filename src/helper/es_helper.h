#ifndef ES_HELPER_H
#define ES_HELPER_H
#include "common.h"

//std::string INDEX_DIR;
//
//unsigned int WINDOW_SIZE;
//
//std::string ES_ADDR;
//unsigned int ES_SHARDS;
//unsigned int ES_REPLICAS;
//unsigned int ES_INDEX_INTERVAL;

namespace es {
	bool has_index(std::string _dirName, unsigned int _windowSize);
	void create_index(std::string _dirName, unsigned int _windowSize, unsigned int _shards, unsigned int _replicas, unsigned int _interval);
	void bulk_index(std::string _dirName, unsigned int _windowSize);
	std::string search(std::vector<std::string> _md5Chunks, std::string _dirName, unsigned int _windowSize);
	namespace data {
		std::string get_query_json(std::vector<std::string> _md5Chunks);
		std::string get_bulk_json(std::string _indexName, std::vector<std::string> _md5Chunks, std::string _fileName);
		std::string get_mapping_json(unsigned int _shards, unsigned int _replicas, unsigned int _interval);
	}
}
#endif
