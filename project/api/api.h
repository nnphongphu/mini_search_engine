#ifndef API_H
#define API_H

#include <string>
#include <map>
#include <vector>
#include "../api/trie.h"

using namespace std;

void getFiles(vector<string>& files);
void getFileContent(string& file, string& content);
void initTrie(vector<string>& files, TrieNode *& root);

#endif