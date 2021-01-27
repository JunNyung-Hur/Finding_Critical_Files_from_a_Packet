#ifndef ES_HELPER
#define ES_HELPER
#include "common.h"

namespace es {
	bool has_index(std::string _dataType, unsigned int _windowSize);
	void create_index(std::string _dataType, unsigned int _windowSize, unsigned int _shards, unsigned int _replicas, unsigned int _interval);
	void bulk_index(std::string _dataType, unsigned int _windowSize);
	std::string search(std::vector<std::string> _md5Chunks, std::string _dataType, unsigned int _windowSize);
	namespace data {
		std::string get_query_json(std::vector<std::string> _md5Chunks);
		std::string get_bulk_json(std::string _indexName, std::vector<std::string> _md5Chunks, std::string _fileName);
		std::string get_mapping_json(unsigned int _shards, unsigned int _replicas, unsigned int _interval);
	}
}
#endif
