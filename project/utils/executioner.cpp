#include <iostream>
#include <map>
#include <sstream>
#include <utility>
#include <vector>
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

vector<int> queryExecution(string &query, TrieNode *&root, vector<string> &files) {
  stringstream ss(query);
  string tmp;

  vector<int> candidates;
  bool firstTerm = true;
  while (ss >> tmp) {
    vector<int> searchAns;
    if (tmp[0] == '"') {
      if (tmp.back() == '"') {
        tmp = tmp.substr(1, tmp.size() - 2);
        if (tmp == "*") continue;

        searchAns = root->searchUnique(tmp);
        intersect(candidates, searchAns);
        continue;
      }

      tmp = tmp.substr(1, tmp.size() - 1);
      while (tmp == "*") ss >> tmp;
      if (tmp == "*\"") break;

      searchAns = root->searchUnique(tmp);
      vector<pair<int, vector<int>>> position;
      for (auto fileId : searchAns)
        position.push_back(make_pair(fileId, getPos(files[fileId], tmp)));

      if (firstTerm) {
        firstTerm = false;
        unite(candidates, searchAns);
      } else
        intersect(candidates, searchAns);

      int delta = 1;
      while (ss >> tmp) {
        bool isFinalTerm = false;

        if (tmp.back() == '"') {
          tmp = tmp.substr(0, tmp.size() - 1);
          isFinalTerm = true;
        }

        if (tmp == "*")
          delta += 1;
        else {
          searchAns = root->searchUnique(tmp);
          vector<pair<int, vector<int>>> newPosition;
          for (auto fileId : searchAns)
            newPosition.push_back(make_pair(fileId, getPos(files[fileId], tmp)));
          updateCandidatesAndPositionInExactQuery(position, candidates, newPosition, delta);
          delta = 1;
        }

        if (isFinalTerm) break;
      }

      continue;
    }

    if (tmp.substr(0, 8) == "intitle:") {
      string required = tmp.substr(8);
      searchAns = root->searchUnique(required);
      searchAns = getInTitleFile(required, searchAns, files);
      if (firstTerm) {
        firstTerm = false;
        unite(candidates, searchAns);
      } else
        intersect(candidates, searchAns);
      continue;
    }

    if (tmp == "OR" || firstTerm) {
      if (firstTerm == false) ss >> tmp;
      firstTerm = false;
      searchAns = root->searchUnique(tmp);
      unite(candidates, searchAns);
      continue;
    }

    if (tmp == "-") {
      ss >> tmp;
      searchAns = root->searchUnique(tmp);
      substract(candidates, searchAns);
      continue;
    }

    if (tmp == "filetype:txt") continue;

    if (tmp == "AND") ss >> tmp;
    if (tmp[0] == '$')
      string sub_tmp = tmp.substr(1, tmp.length() - 1);
    bool isNum = true;
    for (int i = 0; i < sub_tmp.length(); ++i) {
      if (sub_tmp[i] >= '0' && sub_tmp[i] <= '9')
        continue;
      else {
        isNum = false;
        break;
      }
    }
    if (isNum)
      searchAns = root->searchUnique(tmp);
    intersect(candidates, searchAns);
  }
  return candidates;
}
