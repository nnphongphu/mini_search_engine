#ifndef API_H
#define API_H

#include <string>
#include <map>
#include <vector>
#include "../api/trie.h"

void getFiles(std::vector<std::string>& files);
void getFileContent(std::string& file, std::string& content);
void initTrie(std::vector<std::string>& files, TrieNode *& root);
std::vector<std::string> getStopwords();

#endif