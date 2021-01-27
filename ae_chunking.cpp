#include "ae_chunking.h"

std::vector<std::string> ae_chunking(const unsigned char* byteSeq, const unsigned int bytesSize, const unsigned int windowSize) {
	std::vector<std::string> chunkVec;
	unsigned int cursorIdx = 0;
	unsigned int baseIdx = 0;
	int maxValue, maxIdx;
	std::string chunk = "";
	while (cursorIdx < bytesSize){
		maxValue = byteSeq[cursorIdx];
		maxIdx = cursorIdx;
		chunk += static_cast<char>(byteSeq[cursorIdx]);
		cursorIdx++;
		while (cursorIdx < bytesSize) {
			if (byteSeq[cursorIdx] <= maxValue) {
				if (cursorIdx == maxIdx + windowSize) {
					chunkVec.push_back(chunk);
					chunk = "";
					cursorIdx++;
					break;
				}
			}
			else {
				maxValue = byteSeq[cursorIdx];
				maxIdx = cursorIdx;
			}
			chunk += static_cast<char>(byteSeq[cursorIdx]);
			cursorIdx++;
		}
	}
	if (chunk != "") {
		chunkVec.push_back(chunk);
	}
	return chunkVec;
}