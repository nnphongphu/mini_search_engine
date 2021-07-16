#include <iostream>
#include "utils/trie.h"
#include "api/api.h"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	TrieNode* root = new TrieNode();
	initTrie(root);
	cout << "FINISHED INITIALIZING\n";
	TrieNode* search = root->search("dragons");
	if (search == nullptr) return 0;
	for (auto t : search->list) {
		cout << t.first << "\n";
	}
	return 0;
}