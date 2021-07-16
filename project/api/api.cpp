#include "../utils/trie.h"
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


using namespace std;

void initTrie(TrieNode*& root) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    auto start = std::chrono::steady_clock::now();
    ifstream fi;

    fi.open("../data/___index.txt");
    vector< string> files;
    string fileName;
    while (getline(fi, fileName)) files.push_back(fileName);
    fi.close();

    int count = 0;
    for (string file : files) {
        fi.open("../data/" + file);

        if (fi.is_open()) {
            string word;
            int index = 0;
            while (fi >> word) {
                root->insert(word, file, index);
                index += 1;
            }
        }

        fi.close();
        if (count == 100) break;
        count++;
    }

    auto elapsed_time = std::chrono::steady_clock::now() - start;
    std::cout << "Read files in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time)
        .count()
        << " ms.\n";
    return;
}

string getFile(string fileName) {
    ifstream fi("../data/" + fileName);
    string content;
    content.assign(istreambuf_iterator<char>(fi), istreambuf_iterator<char>());
    fi.close();
    return content;
}