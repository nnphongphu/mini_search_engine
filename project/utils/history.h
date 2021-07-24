#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class History {
private:
	ofstream fout;
	ifstream fin;
	fstream f;

public:
	vector<string> getHistory(string s) {
		vector<string> res;
		res.push_back(s);
		
		int i = 1;
		fin.open("history.txt");

		if (fin.is_open()) {
			while (!fin.eof()) {
				string temp; getline(fin, temp);
				if (temp == "" || temp == s || temp.find(s) == -1) continue;

				cout << i++ << ". " << temp << endl;
				res.push_back(temp);
			}
		}
		fin.close();

		fout.open("history.txt", ios::app);
		if(fout.is_open()) fout << s << endl;
		fout.close();

		return res;
	};

	void clearHistory() {
		f.open("history.txt", ios::out | ios::trunc);
		f.close();
	};

	string history(string s) {
		int choice;
		cout << "0. Search" << endl << "1. Show history suggestion" << endl << "OPTION:   ";
		cin >> choice; cin.ignore();
		if (choice) {
			vector<string> res = getHistory(s);
			cin >> choice; cin.ignore();
			if (choice > res.size() - 1) {
				cout << "Invalid" << endl; choice = 0;
			}
			s = res[choice - 1];
		}
		else {
			fout.open("history.txt", ios::app);
			fout << s << endl;
			fout.close();
		}
		return s;
	};
};

#endif // !_HISTORY_H_
