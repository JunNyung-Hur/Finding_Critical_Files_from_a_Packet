#define SAM_DEF
#include "common.h"
#include "bf_helper.h"
#include "es_helper.h"
#include "ae_chunking.h"
#include "utils.h"

int main(void) {
	parse_config();
	if (not es::has_index(DIRECTORY_NAME, WINDOW_SIZE)) {
		es::create_index(DIRECTORY_NAME, WINDOW_SIZE, ES_SHARDS, ES_REPLICAS, ES_INDEX_INTERVAL);
		es::bulk_index(DIRECTORY_NAME, WINDOW_SIZE);
		Sleep(2000);
	}
	
	bloom_filter bf = init_bf(DIRECTORY_NAME, WINDOW_SIZE, BF_ERROR_RATE);
	std::cout << std::endl;
	std::cout << "===== Bloom filter info ============" << std::endl;
	std::cout << "element count : " << bf.element_count() << std::endl;
	std::cout << "hash count : " << bf.hash_count() << std::endl;
	std::cout << "false positive ratio : "<< bf.effective_fpp() << std::endl;
	std::cout << "====================================" << std::endl;

	struct pcap_pkthdr* header;
	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t* pcap;
	const u_char* data;
	unsigned long long packetBytes = 0;
	double duration = 0;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(PCAP_DIR)) {
		std::cout << "Inspect PCAP " << entry.path() << std::endl;
		pcap = pcap_open_offline(entry.path().string().c_str(), errbuff);
		std::vector<std::string> filteredChunks;
		unsigned int chunkSize = 0;
		while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
		{
			clock_t startTime = clock();
			std::vector<std::string> chunkVec = ae_chunking(data, header->caplen, 128);
			packetBytes += header->caplen;
			chunkSize += chunkVec.size();
			for (auto it = chunkVec.begin(); it != chunkVec.end(); it++) {
				if (bf.contains(*it)) {
					std::string md5Chunk = get_md5(*it);
					filteredChunks.push_back(md5Chunk);
				}
			}
			clock_t endTime = clock();
			duration += (double)(endTime - startTime) / CLOCKS_PER_SEC;
		}
		if (filteredChunks.size()) {
			clock_t startTime = clock();
			std::string esRes = es::search(filteredChunks, DIRECTORY_NAME, 128);
			rapidjson::Document resJson;
			resJson.Parse(esRes.c_str());
			clock_t endTime = clock();
			duration += (double)(endTime - startTime) / CLOCKS_PER_SEC;
			rapidjson::Value& hits = resJson["hits"]["hits"];
			if (not hits.Size()) {
				std::cout << "No Searched" << std::endl;
			}
			else {
				for (rapidjson::Value::ConstValueIterator hit = hits.Begin(); hit != hits.End(); ++hit) {
					std::string hitId = (*hit)["_id"].GetString();
					int hitCnt = (int)((*hit)["_score"].GetFloat());
					float hitRatio = ((float)hitCnt / chunkSize) * 100;
					std::cout << "Detected file name : " << hitId << std::endl;
					std::cout << string_format("hits raito : %.2f", hitRatio) << "% ";
					std::cout << string_format("(%d/%d)\n", hitCnt, chunkSize);
				}
			}
		}
		else {
			std::cout << "All Filtered out" << std::endl;
		}
		std::cout << packetBytes * 8 << " " << duration << std::endl;
		std::cout << "Throughput : "<< ((packetBytes*8) / duration) / 1e+6 <<"Mbps"<< std::endl;
		std::cout << std::endl;
	}
	return 0;
}