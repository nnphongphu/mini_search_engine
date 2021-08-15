#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <set>
#include <unordered_map>

class TrieNode {
private:
	const static int N = 256;

public:
	std::vector<int> list;
	TrieNode* child[N];

	TrieNode() {
		for (int i = 0; i < N; i++)
			this->child[i] = nullptr;
	}

	void insert(std::string &s, const int &fileIndex) {
		if (s.empty() || s.size() == 1) return;

		TrieNode* tmp = this;
		for (char& c : s) {
			if (c < 0 || c >= 256) return;
			c = tolower(c);
		}

		for (char c : s) {
			if (tmp->child[c] == nullptr)
				tmp->child[c] = new TrieNode();
			tmp = tmp->child[c];
		}

		if (tmp->list.empty() || tmp->list.back() != fileIndex) tmp->list.push_back(fileIndex);
	}

	TrieNode* search(std::string &s) {
		TrieNode* tmp = this;
		for (char c : s) {
			if (tmp->child[c] == nullptr)
				return nullptr;
			tmp = tmp->child[c];
		}
		return tmp;
	}

	std::vector<int> searchUnique(std::string s) {
		TrieNode* tmp = this;
		for (char c : s) {
			if (tmp->child[c] == nullptr)
				return std::vector<int>();
			tmp = tmp->child[c];
		}

		return tmp->list;
	}

};

#endif TRIE_H