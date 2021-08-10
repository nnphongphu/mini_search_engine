#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "utils.h"

class History {
private:
	std::ofstream fout;
	std::ifstream fin;
	std::fstream f;

public:
	std::vector<std::string> getHistory(std::vector <std::string> list) {
		fin.open("history.txt");

		std::vector<std::string> res;

		if (fin.is_open()) {
			std::map<std::string, int> history;
			while (!fin.eof()) { // To push all word in file history to a map, and set it to 1
				std::string str; std::getline(fin, str);
				history[str] = 1;
			}
			for (auto s : list) {
				std::string s1 = toUpper(s);
				std::string s2 = s;
				if ('a' <= s2[0] && s2[0] <= 'z') s2[0] = s2[0] - 'a' + 'A';
				for (auto& p : history) { // To see if s is substring of p
					if (p.first == "" || ((p.first == s || p.first.find(s) == -1) && (p.first == s1 || p.first.find(s1) == -1) && (p.first == s2 || p.first.find(s2) == -1))) continue;
					else p.second++;
				}
			}

			int i = 1;
			for (auto p : history)
				if (p.second > 1) {
					std::cout << i++ << ". " << p.first << std::endl;
					res.push_back(p.first);
				}
		}
		else std::cout << "Can't open file history.txt\n", std::system("pause");
		fin.close();
		return res;
	};

	void clearHistory() {
		f.open("history.txt", std::ios::out | std::ios::trunc);
		f.close();
	};

	std::string history(std::string s) {
		int choice;
		std::cout << "0. Search" << std::endl << "1. Show history suggestion" << std::endl << "OPTION:   ";
		std::cin >> choice; std::cin.ignore();
		if (choice) {
			std::vector<std::string> list;
			tokenize(s, list); // To token the string
			
			std::vector<std::string> res = getHistory(list);
			
			if (std::find(res.begin(), res.end(), s) != res.end()) { // To check if s existed in file history
				std::cout << res.size() + 1 << ". " << s << std::endl;
				res.push_back(s);
			}

			std::cin >> choice; std::cin.ignore();
			if (choice > res.size() - 1) {
				std::cout << "Invalid" << std::endl; choice = res.size();
			}
			s = res[(std::size_t)choice - 1];
		}
		else {
			fin.open("history.txt");

			std::vector<std::string> res;
			bool isInFile = false;

			if (fin.is_open()) {
				while (!fin.eof()) { // To check if s existed in file.
					std::string str; std::getline(fin, str);
					if (str == s) {
						isInFile = true;
						break;
					}
				}
			}
			fin.close();
			if (!isInFile) { // If it's in file, update file history.
				fout.open("history.txt", std::ios::app);
				fout << s << std::endl;
				fout.close();
			}
		}
		return s;
	};
};

#endif // !_HISTORY_H_