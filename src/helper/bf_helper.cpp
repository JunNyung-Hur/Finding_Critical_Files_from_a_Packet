#include "bf_helper.h"

bloom_filter init_bf(std::string _indexName, int _windowSize, float _errorRate) {
	std::experimental::filesystem::path bfDir(BLOOMFILTER_DIR);
	std::experimental::filesystem::path bfName(_indexName + string_format("_%d_%.0e.bf", _windowSize, _errorRate));
	std::experimental::filesystem::path bf_path = bfDir / bfName;
	int dirFilesCnt = get_number_of_files(INDEX_DIR);
	int processedCnt = 1;
	if (not std::experimental::filesystem::exists(bf_path)) {
		std::set<std::string> chunkSet;
		std::cout << "Create Bloom Filter ..." << std::endl;
		for (const auto& entry : std::experimental::filesystem::directory_iterator(INDEX_DIR)) {
			std::cout << string_format("\rCalculate the number of chunks in files (%d/%d) ... ", processedCnt, dirFilesCnt) << std::flush;
			std::ifstream is(entry.path(), std::ifstream::binary);
			if (is) {
				is.seekg(0, is.end);
				int length = (int)is.tellg();
				is.seekg(0, is.beg);

				unsigned char* buffer = (unsigned char*)malloc(length);
				is.read((char*)buffer, length);
				is.close();

				std::vector<std::string> chunkVec = ae_chunking(buffer, length, _windowSize);
				for (auto it = chunkVec.begin(); it != chunkVec.end(); it++) {
					chunkSet.insert(*it);
				}
				free(buffer);

			}
			processedCnt++;
		}
		std::cout << "done" << std::endl;
		std::cout << "Build bloom filter ... " << std::flush;
		bloom_parameters parameters;
		parameters.projected_element_count = chunkSet.size();
		parameters.false_positive_probability = _errorRate;
		parameters.compute_optimal_parameters();
		bloom_filter bf(parameters);
		for (auto it = chunkSet.begin(); it != chunkSet.end(); it ++) {
			bf.insert(*it);
		}
		std::ofstream ofs(bf_path, std::ios::binary);
		boost::archive::text_oarchive oa(ofs);
		oa << bf;
		ofs.close();
		std::cout << "done" << std::endl;
		return bf;
	}
	else {
		std::cout << "Load bloom filter ... ";
		std::ifstream ifs(bf_path, std::ios::binary);
		boost::archive::text_iarchive ia(ifs);
		bloom_filter bf;
		ia >> bf;
		std::cout << "done" << std::endl;
		return bf;
	}
}
