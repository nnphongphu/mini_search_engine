#include <Windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include "utils.h"

using namespace std;

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