#ifndef UTILS_H_HEADER
#define UTILS_H_HEADER

#include <vector>
#include <string>
#include <utility>
#include <conio.h>
#include <windows.h>
#include "../api/trie.h"

enum ACTION { UP, DOWN, RIGHT, ENTER, BACK };

std::string getTitle(std::string& fileName);
void tokenize(std::string& text, std::vector<std::string>& res);
std::vector<int> queryExecution(std::string& query, TrieNode*& root, std::vector<std::string> &files);
std::vector<int> getPos(std::string& fileName, std::string& key);

void ShowConsoleCursor(bool showFlag);
void gotoxy(int x, int y);
ACTION key(int z);
int front_end(std::vector<std::pair<std::string, std::string>> fileList, COORD position);

std::string toUpper(std::string s);
void highlight(std::string para, std::vector<std::string> keys);

#endif UTILS_H_HEADER