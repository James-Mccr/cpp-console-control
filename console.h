#pragma once

#include <string>

class Console
{
public:
    Console();
    ~Console();

    void moveCursor(int x, int y);
    void print(const char c);
    void print(const std::string& str);
};