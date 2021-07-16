#include <vector>
#include <string>
#include <fstream>
#include "tokenizer.cpp"

using namespace std;

vector<string> getSyno(string key) {
	vector<string> res;
	string line;

	ifstream fin;
	fin.open("synonym.txt");

	if (fin.is_open()) {
		while (!fin.eof()) {

			getline(fin, line);

			vector<string> temp = split(line);

			if (find(temp.begin(), temp.end(), key) != temp.end()) return temp;
		}
	}
	fin.close();

	return res;
}