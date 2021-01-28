#include "es_helper.h"

bool es::has_index(std::string _dirName, unsigned int _windowSize) {
	std::cout << "Check the index exists ... ";
	std::string indexName = _dirName + "_" + std::to_string(_windowSize);
	std::string reqUrl = "http://" + ES_ADDR + "/" + indexName;
	CURL* curl;
	CURLcode res;
	long resCode = -1;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	curl_easy_setopt(curl, CURLOPT_URL, reqUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

	res = curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &resCode);
	curl_easy_cleanup(curl);

	if (resCode == 200) {
		std::cout << "yes" << std::endl;
		return true;
	}
	else {
		std::cout << "no" << std::endl;
		return false;
	}
}

void es::create_index(std::string _dirName, unsigned int _windowSize, unsigned int _shards, unsigned int _replicas, unsigned int _interval) {
	std::string indexName = _dirName + "_" + std::to_string(_windowSize);
	std::cout << "Create index \"" + indexName + "\" ... ";
	std::string reqUrl = "http://" + ES_ADDR + "/" + indexName;
	std::string mappingBody = data::get_mapping_json(_shards, _replicas, _interval);
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");

	curl_easy_setopt(curl, CURLOPT_URL, reqUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, mappingBody.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, mappingBody.size());
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	std::cout << "done" << std::endl;
}

void es::bulk_index(std::string _dirName, unsigned int _windowSize) {
	std::string indexName = _dirName + "_" + std::to_string(_windowSize);
	std::string bulkBody = "";
	for (const auto& entry : std::filesystem::directory_iterator(INDEX_DIR)) {
		std::ifstream is(entry.path(), std::ifstream::binary);
		if (is) {
			is.seekg(0, is.end);
			int length = (int)is.tellg();
			is.seekg(0, is.beg);

			unsigned char* buffer = (unsigned char*)malloc(length);
			is.read((char*)buffer, length);
			is.close();

			std::vector<std::string> md5Chunks;
			std::vector<std::string> chunkVec = ae_chunking(buffer, length, _windowSize);
			for (auto it = chunkVec.begin(); it != chunkVec.end(); it++) {	
				std::string md5Chunk = get_md5(*it);
				md5Chunks.push_back(md5Chunk);
			}
			free(buffer);
			std::string bulkJson = data::get_bulk_json(indexName, md5Chunks, entry.path().filename().string());
			bulkBody += bulkJson + "\n";
		}
	}

	std::string reqUrl = "http://" + ES_ADDR + "/_bulk";
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");

	curl_easy_setopt(curl, CURLOPT_URL, reqUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bulkBody.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, bulkBody.size());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	std::cout << readBuffer << std::endl;
}

std::string es::search(std::vector<std::string> _md5Chunks, std::string _dirName, unsigned int _windowSize) {
	std::string indexName = _dirName + "_" + std::to_string(_windowSize);
	std::string reqUrl = "http://" + ES_ADDR + "/" + indexName + "/_search";
	std::string searchBody = data::get_query_json(_md5Chunks);
	CURL* curl;
	CURLcode resCode;
	std::string readBuffer;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");

	curl_easy_setopt(curl, CURLOPT_URL, reqUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, searchBody.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, searchBody.size());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	resCode = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return readBuffer;
}

std::string es::data::get_query_json(std::vector<std::string> _md5Chunks) {
	rapidjson::Document query;
	query.SetObject();
	rapidjson::Document::AllocatorType& allocator = query.GetAllocator();
	query.AddMember("_source", false, allocator);
	rapidjson::Value _bool(rapidjson::kObjectType);
	rapidjson::Value _should(rapidjson::kObjectType);
	rapidjson::Value _termArray(rapidjson::kArrayType);
	for (auto md5It = _md5Chunks.begin(); md5It != _md5Chunks.end(); md5It++) {
		rapidjson::Value _term(rapidjson::kObjectType);
		rapidjson::Value _data(rapidjson::kObjectType);
		_data.AddMember("data", rapidjson::StringRef((*md5It).c_str()), allocator);
		_term.AddMember("term", _data, allocator);
		_termArray.PushBack(_term, allocator);
	}
	_should.AddMember("should", _termArray, allocator);
	_bool.AddMember("bool", _should, allocator);
	query.AddMember("query", _bool, allocator);

	rapidjson::StringBuffer querybuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(querybuf);
	query.Accept(writer);

	return querybuf.GetString();
}

std::string es::data::get_bulk_json(std::string _indexName, std::vector<std::string> _md5Chunks, std::string _fileName) {
	rapidjson::Document header;
	header.SetObject();
	rapidjson::Document::AllocatorType& allocator = header.GetAllocator();
	rapidjson::Value index(rapidjson::kObjectType);
	index.AddMember("_index", rapidjson::StringRef(_indexName.c_str()), allocator);
	index.AddMember("_id", rapidjson::StringRef(_fileName.c_str()), allocator);
	header.AddMember("index", index, allocator);

	rapidjson::Document body;
	body.SetObject();
	rapidjson::Value dataArray(rapidjson::kArrayType);
	for (auto md5It = _md5Chunks.begin(); md5It != _md5Chunks.end(); md5It++) {
		dataArray.PushBack(rapidjson::StringRef((*md5It).c_str()), allocator);
	}
	body.AddMember("data", dataArray, allocator);


	rapidjson::StringBuffer headerBuf;
	rapidjson::Writer<rapidjson::StringBuffer> headerWriter(headerBuf);
	header.Accept(headerWriter);

	rapidjson::StringBuffer bodyBuf;
	rapidjson::Writer<rapidjson::StringBuffer> bodyWriter(bodyBuf);
	body.Accept(bodyWriter);

	std::string bulkJson = std::string(headerBuf.GetString()) + "\n" + std::string(bodyBuf.GetString());
	return bulkJson;
}

std::string es::data::get_mapping_json(unsigned int _shards, unsigned int _replicas, unsigned int _interval) {
	rapidjson::Document mapping_json;
	mapping_json.SetObject();
	rapidjson::Document::AllocatorType& allocator = mapping_json.GetAllocator();
	rapidjson::Value settings(rapidjson::kObjectType);
	settings.AddMember("refresh_interval", "1s", allocator);
	settings.AddMember("number_of_shards", _shards, allocator);
	settings.AddMember("number_of_replicas", _replicas, allocator);
	rapidjson::Value similarity(rapidjson::kObjectType);
	rapidjson::Value scripted_one(rapidjson::kObjectType);
	scripted_one.AddMember("type", "scripted", allocator);
	rapidjson::Value script(rapidjson::kObjectType);
	script.AddMember("source", "return 1;", allocator);
	scripted_one.AddMember("script", script, allocator);
	similarity.AddMember("scripted_one", scripted_one, allocator);
	settings.AddMember("similarity", similarity, allocator);

	rapidjson::Value mappings(rapidjson::kObjectType);
	mappings.AddMember("dynamic", "strict", allocator);
	rapidjson::Value properties(rapidjson::kObjectType);
	rapidjson::Value data(rapidjson::kObjectType);
	data.AddMember("type", "text", allocator);
	data.AddMember("similarity", "scripted_one", allocator);
	properties.AddMember("data", data, allocator);
	mappings.AddMember("properties", properties, allocator);

	mapping_json.AddMember("settings", settings, allocator);
	mapping_json.AddMember("mappings", mappings, allocator);

	rapidjson::StringBuffer mappingBuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(mappingBuf);
	mapping_json.Accept(writer);

	return mappingBuf.GetString();
}