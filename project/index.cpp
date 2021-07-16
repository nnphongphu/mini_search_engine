#define _HAS_STD_BYTE 0
#include <iostream>
#include "utils/trie.h"
#include <windows.h>
#include "api/api.h"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	TrieNode* root = new TrieNode();
	initTrie(root);
	cout << "--------- FINISHED INITIALIZING ---------\n";
	cout << "--------- WELCOME TO OUR SEARCH ---------\n";
	while (true) {
		cout << "Enter what you want to search: ";
		string s; cin >> s;
		
		int count = 0;
		TrieNode* node = root->search(s);
		cout << "The word " << s << " appears in " << node->list.size() << " documents. Here are the top five:\n";
		for (auto doc : node->list) {
			cout << "Filename: " << doc.first << "\n";
			cout << "Content: ...";
			string content = getFile(doc.first);
			int position = (int)content.find(s);
			for (int i = max(position - 150, 0); i < position; i++) {
				cout << content[i];
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			cout << s;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

			for (auto i = position+s.size(); i < min(position+150, (int)content.size()); i++) {
				cout << content[i];
			}
			cout << "...\n";

			cout << "----------------------------------------\n";
			count += 1;
			if (count == 5) break;
		}
		
	}

	return 0;
}