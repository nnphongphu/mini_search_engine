#include <string>
#include <vector>

using namespace std;

vector<string> split(string text) {
	vector<string> res;
	string temp = "";
	
	for (int i = 0; i < text.size(); i++) {
		if ((' ' <= text[i] && text[i] <= '/') || text[i] == ':' || text[i] == ';' || text[i] == '?') {
			if (find(res.begin(), res.end(), temp) == res.end()) {
				res.push_back(temp);
			}
			temp = "";
		}
		else temp += tolower(text[i]);
	}

	return res;
}

bool isStopWord(string word) {
    ifstream fin;
    fin.open("stopword.txt");
    bool isstopword = false;
    if (fin.is_open()) {
        vector<string> stopwords;
        string get;
        while (!fin.eof()) {
            getline(fin, get);
            stopwords.push_back(get);
        }
        for (int i = 0; i < stopwords.size(); i++) {
            if (word.compare(stopwords[i]) == 0) {
                isstopword = true;
                break;
            }
        }
    }
    fin.close();
    return isstopword;
}