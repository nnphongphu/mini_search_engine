#define _HAS_STD_BYTE 0
#include <iostream>
#include "api/trie.h"
#include <windows.h>
#include "api/api.h"
#include "utils/utils.h"
#include "utils/front_end.cpp"
#include "utils/history.h"
#include "utils/highlight.cpp"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	vector<string> files;
	getFiles(files);

	TrieNode* root = new TrieNode();
	initTrie(files, root);
	
	while (true) {
		cout << "--------- FINISHED INITIALIZING ---------\n";
		cout << "--------- WELCOME TO OUR SEARCH ---------\n";

		cout << "Enter what you want to search: ";
		string query;
		getline(cin, query);

		History history;
		query = history.history(query);
		
		vector<int> list = queryExecution(query, root, files);

		int count = 1;
		
		vector<pair<string, string>> fileList;

		for (int id : list) {

			string file = files[id];
			string content;
			getFileContent(file, content);
			string title = getTitle(file);

			fileList.push_back({ file, title });

			count += 1;
			if (count == 5) break;
		}
		
		cout << "Below are top 5 results, please choose file you want to see (press BACK to find another world).\n";

		int result = front_end(fileList, { 0, 6 });

		system("CLS");
		if (result == fileList.size()) continue;
		else {
			// highlight() parameter is a string which is file we print out, and a vector<string> which is list of keywords.
		}
	}

	return 0;
}