#ifndef UTILS_H_HEADER
#define UTILS_H_HEADER

#include <vector>
#include <string>
#include <utility>
#include <conio.h>
#include <windows.h>
#include "../api/trie.h"

enum ACTION { UP, DOWN, RIGHT, ENTER, BACK };

void normalize(std::string& token);
std::vector<std::string> getTopFive(std::vector<std::string> files, std::vector<std::string> words, std::vector<int> list);
std::string getTitle(std::string& fileName);
void tokenize(std::string& text, std::vector<std::string>& res);
std::pair<std::vector<int>, std::vector<std::string>> queryExecution(std::string& query, TrieNode*& root, std::vector<std::string> &files, std::vector<std::string>& stopwords);
std::vector<int> getPos(std::string& fileName, std::string& key);
bool isStopword(std::string& word, std::vector<std::string>& stopwords);
std::vector<std::string> getSyno(std::string key);

void ShowConsoleCursor(bool showFlag);
void gotoxy(int x, int y);
ACTION key(int z);
int front_end(std::vector<std::pair<std::string, std::string>> fileList, COORD position);

std::string toUpper(std::string s);
void highlight(std::string para, std::vector<std::string> keys);
std::string toLower(std::string s);

#endif UTILS_H_HEADER