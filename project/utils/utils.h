#ifndef UTILS_H_HEADER
#define UTILS_H_HEADER

#include <vector>
#include <string>
#include "../api/trie.h"

using namespace std;

string getTitle(string& fileName);
void tokenize(string& text, vector<string>& res);
vector<int> queryExecution(string& query, TrieNode*& root, vector<string> &files);
vector<int> getPos(string& fileName, string& key);

#endif UTILS_H_HEADER