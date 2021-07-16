#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <map>
#include <unordered_map>

using namespace std;

class TrieNode {
private:
	const static int N = 500;

public:
	map<string, vector<int>> list;
	TrieNode* child[N];

	TrieNode() {
		for (int i = 0; i < N; i++)
			this->child[i] = nullptr;
	}

	void insert(string s, string file, int index) {
		if (s.empty() || s.size() == 1) return;

		TrieNode* tmp = this;
		for (char& c : s) {
			if (c < 0 || c > 200) return;
			c = tolower(c);
		}

		if (s[0] == '"')
			s.erase(s.begin());

		if (s[s.size() - 1] == '.' || s[s.size() - 1] == ',' || s[s.size()-1] == '"')
			s.erase(prev(s.end()));

		for (char c : s) {
			if (tmp->child[c] == nullptr)
				tmp->child[c] = new TrieNode();
			tmp = tmp->child[c];
		}

		tmp->list[file].push_back(index);
	}

	TrieNode* search(string s) {
		TrieNode* tmp = this;
		for (char c : s) {
			if (tmp->child[c] == nullptr)
				return nullptr;
			tmp = tmp->child[c];
		}
		return tmp;
	}
};

#endif //TRIE_H