#include <iostream>
#include <map>
#include <sstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "../api/api.h"
#include "../api/trie.h"
#include "utils.h"

using namespace std;

typedef vector<pair<int, vector<int>>> vpv;

void intersect(vector<int> &a, const vector<int> &b) {
  vector<int> ans;
  for (int file : a) {
    if (find(b.begin(), b.end(), file) != b.end()) {
      ans.push_back(file);
    }
  }
  a = ans;
}

void unite(vector<int> &a, const vector<int> &b) {
  for (int file : b) {
    if (find(a.begin(), a.end(), file) == a.end()) {
      a.push_back(file);
    }
  }
}

void substract(vector<int> &a, const vector<int> &b) {
  vector<int> ans;
  for (int file : a) {
    if (find(b.begin(), b.end(), file) == b.end()) {
      ans.push_back(file);
    }
  }
  a = ans;
}

vector<int> getInTitleFile(string &required, vector<int> &candidates, vector<string> &files) {
  vector<int> ans;
  for (int id : candidates) {
    string title = getTitle(files[id]);
    vector<string> words;
    tokenize(title, words);
    if (find(words.begin(), words.end(), required) != words.end())
      ans.push_back(id);
  }

  return ans;
}

void updateCandidatesAndPositionInExactQuery(vpv &position, vector<int> &candidates, vpv &newPosition, int delta) {
  vector<pair<int, vector<int>>> updatedPosition;
  vector<int> updatedCandidates;
  for (auto [aId, aPos] : position) {
    for (auto [bId, bPos] : newPosition) {
      if (aId == bId) {
        int l = 0, r = 0;
        vector<int> newPos;
        while (l < aPos.size() && r < bPos.size()) {
          if (aPos[l] + delta == bPos[r]) {
            newPos.push_back(bPos[r]);
            l += 1;
            r += 1;
          } else if (aPos[l] + delta < bPos[r])
            l += 1;
          else
            r += 1;
        }
        if (newPos.size()) {
          updatedPosition.push_back(make_pair(aId, newPos));
          updatedCandidates.push_back(aId);
        }
      }
    }
  }
  position = updatedPosition;
  candidates = updatedCandidates;
}

bool isNum(string s) {
	bool ans = true;
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] >= '0' && s[i] <= '9')
			continue;
		else {
			ans = false;
			break;
		}
	}
	return ans;
}

pair<vector<int>, vector<string>> queryExecution(std::string& query, TrieNode*& root, std::vector<std::string>& files, std::vector<std::string>& stopwords) {
stringstream ss(query);
string tmp;

vector<int> candidates;
vector<string> highlights;
bool useOr = true;
while (ss >> tmp) {
	vector<int> searchAns;
	if (tmp[0] == '"') {
		if (tmp.back() == '"') {
			tmp = tmp.substr(1, tmp.size()-2);
			if (tmp == "*") continue;

			transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
			searchAns = root->searchUnique(tmp);
			highlights.push_back(tmp);
			if (useOr) unite(candidates, searchAns);
			else intersect(candidates, searchAns);
			useOr = false;
			continue;
		}

		tmp = tmp.substr(1);
		while (tmp == "*") ss >> tmp;
		if (tmp == "*\"") break;
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		searchAns = root->searchUnique(tmp);
		highlights.push_back(tmp);
		vector< pair<int, vector<int>> > position;
		for (auto fileId : searchAns)
			position.push_back(make_pair(fileId, getPos(files[fileId], tmp)));

		vector<int> tCandidates;
		unite(tCandidates, searchAns);

		int delta = 1;
		while (ss >> tmp) {
			transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
			bool isFinalTerm = false;

			if (tmp.back() == '"') {
				tmp = tmp.substr(0, tmp.size() - 1);
				isFinalTerm = true;
			}

			if (tmp == "*") delta += 1;
			else {
				highlights.push_back(tmp);
				searchAns = root->searchUnique(tmp);
				vector< pair<int, vector<int>> > newPosition;
				for (auto fileId : searchAns)
					newPosition.push_back(make_pair(fileId, getPos(files[fileId], tmp)));
				updateCandidatesAndPositionInExactQuery(position, tCandidates, newPosition, delta);

				delta = 1;
			}
			if (isFinalTerm) break;
		}

		if (useOr) unite(candidates, tCandidates);
		else intersect(candidates, tCandidates);
		useOr = false;
		continue;
	}

	if (tmp.substr(0, 8) == "intitle:") {
		string required = tmp.substr(8);
		transform(required.begin(), required.end(), required.begin(), [](char c) { return std::tolower(c); });
		highlights.push_back(required);
		searchAns = root->searchUnique(required);
		searchAns = getInTitleFile(required, searchAns, files);
		if (useOr) {
			useOr = false;
			unite(candidates, searchAns);
		}
		else intersect(candidates, searchAns);
		useOr = false;
		continue;
	}

	if (tmp[0] == '~') {
		tmp = tmp.substr(1, tmp.size() - 1);
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		vector<string> synonyms = getSyno(tmp);
		vector<int> searchAns = root->searchUnique(tmp);
		highlights.push_back(tmp);
		for (string term : synonyms) {
			vector<int> subAns = root->searchUnique(tmp);
			unite(searchAns, subAns);
			highlights.push_back(term);
		}
		if (useOr) unite(candidates, searchAns);
		else  intersect(candidates, searchAns);

		useOr = false;
		continue;
	}

	if (tmp.substr(0, 9) == "filetype:") {
		tmp = tmp.substr(9, tmp.size() - 9);
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		if (tmp == "txt") continue;
		else {
			candidates.clear();
			break;
		}
	}

	if (tmp == "OR") {
		useOr = true;
		continue;
	}

	if (tmp[0] == '-') {
		tmp = tmp.substr(1, tmp.size() - 1);
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		highlights.push_back(tmp);
		searchAns = root->searchUnique(tmp);
		substract(candidates, searchAns);
		continue;
	}

	if (tmp[0] != '+') {
		transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
		if (isStopword(tmp, stopwords)) continue;
	}
	else tmp = tmp.substr(1, tmp.size() - 1);

	if (tmp == "AND") continue;

	transform(tmp.begin(), tmp.end(), tmp.begin(), [](char c) { return std::tolower(c); });
	if (tmp[0] == '$') {
		if (tmp.find("..") != std::string::npos) {
			int found = tmp.find("..");
			int l = stoi(tmp.substr(1, found));
			int r = stoi(tmp.substr(found + 2));

			vector<int> searchAns;
			for (int i = l; i <= r; i++) {
				string cur = "$" + to_string(i);
				unite(searchAns, root->searchUnique(cur));
				highlights.push_back(cur);
			}

			if (useOr) unite(candidates, searchAns);
			else intersect(candidates, searchAns);

			continue;
		}

		vector<int> searchAns;
		if (isNum(tmp.substr(1))) {
			searchAns = root->searchUnique(tmp);
		    highlights.push_back(tmp);
		    if (useOr) unite(candidates, searchAns);
		    else intersect(candidates, searchAns);
		}
	}

	highlights.push_back(tmp);
	searchAns = root->searchUnique(tmp);
	if (useOr) unite(candidates, searchAns);
	else intersect(candidates, searchAns);

	useOr = false;
}

return make_pair(candidates, highlights);
}
