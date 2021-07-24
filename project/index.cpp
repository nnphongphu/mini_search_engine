#define _HAS_STD_BYTE 0
#include <iostream>
#include "api/trie.h"
#include <windows.h>
#include "api/api.h"
#include "utils/utils.h"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	vector<string> files;
	getFiles(files);

	TrieNode* root = new TrieNode();
	initTrie(files, root);

	cout << "--------- FINISHED INITIALIZING ---------\n";
	cout << "--------- WELCOME TO OUR SEARCH ---------\n";

	while (true) {
		cout << "Enter what you want to search: ";
		string query;
		getline(cin, query);
		
		vector<int> list = queryExecution(query, root, files);

		int count = 1;
		for (int id : list) {

			string file = files[id];
			string content;
			getFileContent(file, content);
			string title = getTitle(file);

			cout << "Filename: " << file << "\n";
			cout << "Title: " << title << "\n";

			count += 1;
			if (count == 5) break;
		}
		
	}

	return 0;
}