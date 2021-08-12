#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../api/api.h"
#include "utils.h"
#include <queue>
#include <algorithm>
#include "highlight.cpp"

using namespace std;

void tokenize(string &text, vector<string> &res) {
	res.reserve(min(9000, (int)text.size()));
	stringstream ss(text);
	string tmp;
	while (ss >> tmp) {
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		while (tmp.size() && !((tmp.back() >= 'a' && tmp.back() <= 'z') || (tmp.back() >= '0' && tmp.back() <= '9'))) tmp.pop_back();
		while (tmp.size() && (!((tmp[0] >= 'a' && tmp[0] <= 'z') || (tmp[0] >= '0' && tmp[0] <= '9')) && !(tmp[0] == '$' || tmp[0] == '#'))) tmp = tmp.substr(1);
		res.push_back(tmp);
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
			replace(line.begin(), line.end(), ',', ' ');
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

	return title;
}

vector<int> getPos(string& fileName, string& key) {
	string content;
	getFileContent(fileName, content);

	vector<string> token;
	tokenize(content, token);

	vector<int> pos;
	for (int i = 0; i < (int)token.size(); i++) {
		if (token[i] == key) pos.push_back(i);
	}

	return pos;
}

bool isStopword(string& word, vector<string>& stopwords) {
	return binary_search(stopwords.begin(), stopwords.end(), word);
}

vector<string> getTopFive(vector<string> files, vector<string> words, vector<int> list) {
	priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> q;

	for (int id : list) {
		int count = 0;
		size_t pos = 0;
		ifstream fin; fin.open("../data/" + files[id]);
		if (!fin.is_open()) continue;
		string para;
		getline(fin, para, '|');
		while (pos != string::npos) {
			size_t MIN = pos + 1; string key; bool first = false;
			for (string word : words) {
				// To find key
				string tStr = word;
				size_t pos_ = para.find(tStr, MIN);
				if (pos_ < pos || !first) first = true, pos = pos_, key = tStr;

				// To find upper key, i.e in -> IN
				tStr = toUpper(word);
				pos_ = para.find(tStr, MIN);
				if (pos_ < pos) first = true, pos = pos_, key = tStr;

				// To find key with first char that is upper char, i.e in -> In
				tStr = word;
				if ('a' <= tStr[0] && tStr[0] <= 'z') tStr[0] = tStr[0] - 'a' + 'A';
				pos_ = para.find(tStr, MIN);
				if (pos_ < pos) first = true, pos = pos_, key = tStr;
			}
			if (pos != string::npos && !((pos != 0 && ((para[pos - 1] >= '0' && para[pos - 1] <= '9') || (para[pos - 1] >= 'A' && para[pos - 1] <= 'Z') || (para[pos - 1] >= 'a' && para[pos - 1] <= 'z'))) ||
				(pos + key.size() < para.size() && ((para[pos + key.size()] >= '0' && para[pos + key.size()] <= '9') || (para[pos + key.size()] >= 'A' && para[pos + key.size()] <= 'Z') || (para[pos + key.size()] >= 'a' && para[pos + key.size()] <= 'z'))))) count++;
		}
		if (!count) continue;
		if (q.size() < 5) q.push(make_pair(count, files[id]));
		else {
			if (q.top().first < count) {
				q.pop(); q.push(make_pair(count, files[id]));
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

string toUpper(string s) {
	for (int i = 0; i < s.size(); i++) {
		char& c = s[i];
		if ('a' <= c && c <= 'z') c = c - 'a' + 'A';
	}
	return s;
};

void highlight(string para, vector<string> keys) {
	size_t pos = 0;
	do {
		bool first = false;
		unsigned int temp = pos; string key = "";
		for (auto key_ : keys) {
			// To find key
			string tStr = key_;
			size_t pos_ = para.find(tStr, temp);
			if (pos_ < pos || !first) first = true, pos = pos_, key = tStr;

			// To find upper key, i.e in -> IN
			tStr = toUpper(key_);
			pos_ = para.find(tStr, temp);
			if (pos_ < pos) first = true, pos = pos_, key = tStr;

			// To find key with first char that is upper char, i.e in -> In
			tStr = key_;
			if ('a' <= tStr[0] && tStr[0] <= 'z') tStr[0] = tStr[0] - 'a' + 'A';
			pos_ = para.find(tStr, temp);
			if (pos_ < pos) first = true, pos = pos_, key = tStr;
		}
		if (pos != string::npos) {
			if (pos != temp) { // Print from the previous key to new key
				cout << para.substr(temp, pos - temp);
			}
			if ((pos != 0 && ((para[pos - 1] >= '0' && para[pos - 1] <= '9') || (para[pos - 1] >= 'A' && para[pos - 1] <= 'Z') || (para[pos - 1] >= 'a' && para[pos - 1] <= 'z'))) ||
				(pos + key.size() < para.size() && ((para[pos + key.size()] >= '0' && para[pos + key.size()] <= '9') || (para[pos + key.size()] >= 'A' && para[pos + key.size()] <= 'Z') || (para[pos + key.size()] >= 'a' && para[pos + key.size()] <= 'z'))))
			{	// Just in case the new key is in another word. Then print key.
				cout << key;
				pos += key.size();
				continue;
			}
		}
		else { // If there isn't any word. Print to the end of file and stop.
			cout << para.substr(temp);
			return;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // To highlight.
		cout << key;
		pos += key.size();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	} while (pos < para.size());
}

std::string toLower(std::string s) {
	for (auto& c : s) {
		if ('A' <= c && c <= 'Z') c = c - 'A' + 'a';
	}
	return s;
}