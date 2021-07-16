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