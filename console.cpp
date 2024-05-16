#include "console.h"
#include <curses.h>
#include <iostream>
#include <ncurses.h>

Console::Console() 
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
}

Console::~Console()
{
    curs_set(1);
    endwin();
}

void Console::moveCursor(int x, int y)
{
    print("\033[" + std::to_string(x) + ";" + std::to_string(y) + "H");
}

void Console::print(const char c)
{
    std::cout << c << std::flush;
}

void Console::print(const std::string& str)
{
    std::cout << str << std::flush;
}

