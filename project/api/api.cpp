#include "../utils/utils.h"
#include "trie.h"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <map>
#include <sstream>
#include <cstdio>
#include <filesystem>


using namespace std;
namespace fs = std::filesystem;

void getFiles(vector<string> &files) {
    ifstream fi("../data/___index.txt");
    string file;
    while (getline(fi, file))
        files.push_back(file);
    fi.close();
}

inline void getFileContent(string& file, string& content) {
    ifstream fi("../data/" + file);
    if (fi.is_open() == false) cout << "ERROR\n";
    fi.seekg(0, std::ios::end);
    content.reserve(fi.tellg());
    fi.seekg(0, std::ios::beg);
    content.assign(istreambuf_iterator<char>(fi), istreambuf_iterator<char>());
}

void initTrie(vector<string>& files, TrieNode*& root) {
    auto start = std::chrono::steady_clock::now();
 
    ifstream fi;
    int fileIndex = 0;
    for (string file : files) {
        fi.open("../data/" + file);

        if (fi) {
            string content;
            getFileContent(file, content);

            vector<string> token;
            tokenize(content, token);
            
            for (string tok : token) {
                root->insert(tok, fileIndex);
            }
        }

        fi.close();

        if (fileIndex - 100 == 0) break;
        fileIndex += 1;
    }

    auto elapsed_time = std::chrono::steady_clock::now() - start;
    std::cout << "Read files in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time)
        .count()
        << " ms.\n";
    return;
}

vector<string> getStopwords() {
    vector<string> ans;
    ifstream fi("stopwords.txt");
    string tmp;
    while (fi >> tmp) ans.push_back(tmp);
    sort(ans.begin(), ans.end());
    return ans;
}