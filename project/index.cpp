#define _HAS_STD_BYTE 0
#include <iostream>
#include "api/trie.h"
#include <windows.h>
#include <utility>
#include <chrono>
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

	auto start = std::chrono::steady_clock::now();

	TrieNode* root = new TrieNode();
	initTrie(files, root);

	auto elapsed_time = std::chrono::steady_clock::now() - start;
	std::cout << "Indexing in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time)
		.count()
		<< " ms.\n";

	vector<string> stopwords = getStopwords();
	bool firstRender = true;
	while (true) {
		if (firstRender == false) system("CLS");
		else firstRender = false;

		cout << "--------- FINISHED INITIALIZING ---------\n";
		cout << "--------- WELCOME TO OUR SEARCH ---------\n";

		cout << "Enter what you want to search: ";
		string query;
		getline(cin, query);

		query = toLower(query);

		History history;
		query = history.history(query);

		system("CLS");

		vector<int> list;
		vector<string> highlights;
		tie(list, highlights) = queryExecution(query, root, files, stopwords);

		vector<string> listFile = getTopFive(files, highlights, list);

		if (listFile.size() == 0) {
			cout << "We can't find your query!\n";
			system("pause");
			string temp; getline(cin, temp);
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

		int z = 13;

		do {
			system("CLS");
			cout << "Below are top 5 results, please choose file you want to see (press BACK to find another word).\n";

			int result = front_end(fileList, { 0,  3 });

			system("CLS");
			if (result == fileList.size()) z = 13;
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
				cout << "Press BACK to get back, ENTER to continue!";
				z = _getch();
			}
		} while (key(z) == BACK);
	}
	return 0;
}