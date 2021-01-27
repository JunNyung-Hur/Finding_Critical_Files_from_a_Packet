#include "utils.h"

void print_chunk_arr(std::vector<std::vector<unsigned char>> _chunk_arr) {
	std::vector<std::vector<unsigned char>>::iterator chunk_iter;
	std::vector<unsigned char>::iterator string_iter;
	for (chunk_iter = _chunk_arr.begin(); chunk_iter != _chunk_arr.end(); chunk_iter++) {
		for (string_iter = chunk_iter->begin(); string_iter != chunk_iter->end(); string_iter++) {
			printf("%c", *string_iter);
		}
		printf("\n");
	}
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string get_md5(std::string _data) {
	unsigned char result[MD5_DIGEST_LENGTH];
	MD5((unsigned char*)_data.c_str(), _data.size(), result);
	std::ostringstream sout;
	sout << std::hex << std::setfill('0');
	for (long long c : result)
	{
		sout << std::setw(2) << (long long)c;
	}
	std::string md5Chunk = sout.str();
	return md5Chunk;
}