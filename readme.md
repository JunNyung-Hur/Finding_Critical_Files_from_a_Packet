# Finding Critical Files from a Packet (IEEE INFOCOM'21)
https://ieeexplore.ieee.org/document/9488914

## Purpose
- To check if there are any important files in session captured pcap file

## Dependencies
The following package is required to build this project:
- boost >= 1.6.x
- libopenssl
- libcurl
- libpcap
- rapidjson 
- elasticsearch >= 7.x

## Building
```
$ make
```

## Setup
- edit "config.txt" in project root
```
# Constants
CHUNKING_WINDOW_SIZE = 128       # Making this option larger provides more accurate search quality but fewer search files  
BLOOMFILTER_ERROR_RATE = 0.001   # It depends on your system memory, but 0.001 is enough.

# Directories location
INDEX_DIR = /home/infosec/infocom/raw/malware/index   # Directory path for indexing important files 
BLOOMFILTER_DIR = /home/infosec/infocom/bf/c++        # Directory path for storing bloom filter (please make it!)
PCAP_DIR = /home/infosec/infocom/pcap/socket/malware  # Directory path where pcap files to inspect are stored

# Elasticsearch configuration
ES_ADDR = localhost:9200     # your Elasticsearch's host and port
ES_SHARDS = 5 
ES_REPLICAS = 0
ES_INDEX_INTERVAL = 1        # Waiting time for indexing
INDEX_NAME = malware         # name for distinguish index datatset

```


## Usage
1. Place your target important files to the "INDEX_DIR" set in "config.txt" to check for leaks
2. Place your PCAP files to the "PCAP_DIR" set in "config.txt" to check if there are any critical files inside the pcap
3. Make directory to store bloom filter and specify "BLOOM_FILTER_DIR" option in "config.txt"
4. run "main"
```
$ ./main
```
*  Do not use the "INDEX_NAME" for different dataset at the same time

## Screenshots
<img src = "https://user-images.githubusercontent.com/13129849/106223068-183e1a80-6224-11eb-9b5b-0ef970235d03.png" width="800px">
