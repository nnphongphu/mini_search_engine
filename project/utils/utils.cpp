#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../api/api.h"
#include "utils.h"
#include <queue>

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

vector<string> getTopFive(vector<string> files, vector<string> words) {
	priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> q;

	for (string file : files) {
		int count = 0;
		size_t pos = 0;
		ifstream fin; fin.open("../data/" + file);
		if (!fin.is_open()) continue;
		string para;
		getline(fin, para, '|');
		while (pos != string::npos) {
			size_t MIN = pos; string key;
			for (string word : words) {
				size_t temp = para.find(word, pos + 1);
				if (MIN == pos || MIN > temp) MIN = temp, key = word;
			}
			pos = MIN;
			if (pos != string::npos && !((pos != 0 && ((para[pos - 1] >= '0' && para[pos - 1] <= '9') || (para[pos - 1] >= 'A' && para[pos - 1] <= 'Z') || (para[pos - 1] >= 'a' && para[pos - 1] <= 'z'))) ||
				(pos + key.size() < para.size() && ((para[pos + key.size()] >= '0' && para[pos + key.size()] <= '9') || (para[pos + key.size()] >= 'A' && para[pos + key.size()] <= 'Z') || (para[pos + key.size()] >= 'a' && para[pos + key.size()] <= 'z'))))) count++;
		}
		if (!count) continue;
		if (q.size() < 5) q.push(make_pair(count, file));
		else {
			if (q.top().first < count) {
				q.pop(); q.push(make_pair(count, file));
			}
		}
		fin.close();
	}

	vector<string> res;
	while (!q.empty()) {
		res.push_back(q.top().second);
		q.pop();
	}
	return res;
}