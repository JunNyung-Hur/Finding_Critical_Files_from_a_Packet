CXX=g++
CFLAGS=-std=c++17
INC=-Isrc -Isrc/algorithm -Isrc/utils -Isrc/helper -Ilib
LIBS=-lstdc++fs -lcurl -lcrypto -lboost_serialization -lpcap
OBJS=main.o ae_chunking.o bf_helper.o es_helper.o utils.o

all: main.o
	$(CXX) $(OBJS) -o main $(LIBS)
	rm -f $(OBJS)

ae_chunking.o: src/algorithm/ae_chunking.h
	$(CXX) $(CFLAGS) $(INC) -c src/algorithm/ae_chunking.cpp

bf_helper.o: src/helper/bf_helper.h lib/bloomfilter.hpp
	$(CXX) $(CFLAGS) $(INC) -c src/helper/bf_helper.cpp

es_helper.o: src/helper/es_helper.h
	$(CXX) $(CFLAGS) $(INC) -c src/helper/es_helper.cpp

utils.o: src/utils/utils.h src/utils/utils.hpp
	$(CXX) $(CFLAGS) $(INC) -c src/utils/utils.cpp

main.o: ae_chunking.o bf_helper.o es_helper.o utils.o
	$(CXX) $(CFLAGS) $(INC) -c src/main.cpp

.PHONY: clean
clean:
	rm -f $(OBJS) main
