#define _HAS_STD_BYTE 0
#include <iostream>
#include "api/trie.h"
#include <windows.h>
#include <utility>
#include "api/api.h"
#include "utils/utils.h"
#include "utils/history.h"

using namespace std;

History temp;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	vector<string> files;
	getFiles(files);

	TrieNode* root = new TrieNode();
	initTrie(files, root);

	vector<string> stopwords = getStopwords();
	while (true) {
		system("CLS");

		cout << "--------- FINISHED INITIALIZING ---------\n";
		cout << "--------- WELCOME TO OUR SEARCH ---------\n";

		cout << "Enter what you want to search: ";
		string query;
		getline(cin, query);

		string tQuery = query;
		query = toLower(query);

		History history;
		query = history.history(query);

		system("CLS");

		vector<int> list;
		vector<string> highlights;
		tie(list, highlights) = queryExecution(tQuery, root, files, stopwords);

		vector<string> listFile = getTopFive(files, highlights, list);

		if (listFile.size() == 0) {
			cout << "We can't find your query!\n";
			system("pause");
			continue;
		}

		vector<pair<string, string>> fileList;

		for (string f : listFile) {
			string file = f;
			string content;
			getFileContent(file, content);
			string title = getTitle(file);

			fileList.insert(fileList.begin(), { file, title });
		}

		cout << "Below are top 5 results, please choose file you want to see (press BACK to find another word).\n";

		int result = front_end(fileList, { 0,  3 });

		system("CLS");
		if (result == fileList.size()) continue;
		else {
			ifstream fin;
			fin.open("../data/" + listFile[result]);
			if (!fin.is_open()) continue;
			string para; getline(fin, para, '|'); 
			cout << "Press ENTER to see file!";
			cin.ignore();
			system("CLS");
			highlight(para, highlights);
			cout << endl;
			system("pause");
		}
	}

	return 0;
}