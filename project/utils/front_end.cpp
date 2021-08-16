#include <iostream>
#include <vector>
#include <utility>
#include <conio.h>
#include <windows.h>
#include "utils.h"

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { SHORT(x), SHORT(y) };
    SetConsoleCursorPosition(h, c);
}

ACTION key(int z) {
    if (z == 224) {
        char c;
        c = _getch();
        if (c == 72) return UP;
        if (c == 80) return DOWN;
    }
    else if (z == 13) return ENTER;
    else if (z == 8) return BACK;
    return RIGHT;
}

int front_end(std::vector<std::pair<std::string, std::string>> fileList, COORD position) {
    ShowConsoleCursor(false);

    int status = 0;

    int location[5] = { 0, 2, 4, 6, 8 };

    for (int i = 0; i < fileList.size(); i++) {
        gotoxy(position.X, position.Y + i + location[i]);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (i == status) ? 176 : 15);
        std::cout << i + 1 << ". Filename: " << fileList[i].first << " - Title: " << fileList[i].second;
    }

    while (true) {

        int z = _getch(); int action = key(z);
        switch (action) {
        case UP:
        case DOWN: {
            gotoxy(position.X, position.Y + status + location[status]);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            std::cout << status + 1 << ". Filename: " << fileList[status].first << " - Title: " << fileList[status].second;

            if (action == UP) {
                if (!status) status = (int)fileList.size() - 1;
                else status--;
            }
            else if (action == DOWN) {
                if (status == (int)fileList.size() - 1) status = 0;
                else status++;
            }
            break;
        }

        case ENTER:
        case BACK: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            system("CLS");

            ShowConsoleCursor(true);

            return (action == ENTER) ? status : fileList.size();
        }
        };

        gotoxy(position.X, position.Y + status + location[status]);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176);
        std::cout << status + 1 << ". Filename: " << fileList[status].first << " - Title: " << fileList[status].second;
    }
}