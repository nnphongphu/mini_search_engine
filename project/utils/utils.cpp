#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../api/api.h"
#include "utils.h"

using namespace std;

void tokenize(string &text, vector<string> &res) {
	string temp = "";
	res.reserve(min(9000, (int)text.size()));

	for (char c : text) {
		if ((' ' <= c && c <= '/') || c == ':' || c == ';' || c == '?') {
			res.emplace_back(temp);
			temp = "";
		}
		else temp += tolower(c);
	}
	return ;
}

vector<string> getSyno(string key) {
	vector<string> res;
	string line;

	ifstream fin;
	fin.open("synonym.txt");

	if (fin.is_open()) {
		while (!fin.eof()) {

			getline(fin, line);

			vector<string> temp;
			tokenize(line, temp);

			if (find(temp.begin(), temp.end(), key) != temp.end()) return temp;
		}
	}
	fin.close();

	return res;
}

string getTitle(string& fileName) {
	string content;
	getFileContent(fileName, content);

	stringstream ss(content);
	string title;
	while (title.size() <= 1) {
		getline(ss, title);
	}

	if (title.size() > 200) {
		return title.substr(0, title.find("."));
	}
	else return title;
}

vector<int> getPos(string& fileName, string& key) {
	string content;
	getFileContent(fileName, content);

	vector<string> token;
	tokenize(content, token);

	vector<int> pos;
	for (int i = 0; i < (int)token.size(); i++)
		if (token[i] == key) pos.push_back(i);

	return pos;
}

bool isStopword(string& word, vector<string>& stopwords) {
	return binary_search(stopwords.begin(), stopwords.end(), word);
}