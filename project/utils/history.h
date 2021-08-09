#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class History {
private:
	std::ofstream fout;
	std::ifstream fin;
	std::fstream f;

public:
	std::vector<std::string> getHistory(std::string s) {
		std::vector<std::string> res;
		res.push_back(s);
		
		int i = 1;
		fin.open("history.txt");

		if (fin.is_open()) {
			while (!fin.eof()) {
				std::string temp; getline(fin, temp);
				if (temp == "" || temp == s || temp.find(s) == -1) continue;

				std::cout << i++ << ". " << temp << std::endl;
				res.push_back(temp);
			}
		}
		fin.close();

		fout.open("history.txt", std::ios::app);
		if(fout.is_open()) fout << s << std::endl;
		fout.close();

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
			std::vector<std::string> res = getHistory(s);
			std::cin >> choice; std::cin.ignore();
			if (choice > res.size() - 1) {
				std::cout << "Invalid" << std::endl; choice = 0;
			}
			s = res[(std::size_t)choice - 1];
		}
		else {
			fout.open("history.txt", std::ios::app);
			fout << s << std::endl;
			fout.close();
		}
		return s;
	};
};

#endif // !_HISTORY_H_
