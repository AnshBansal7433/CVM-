#pragma once

#include <string>
#include <vector>
#include <map>

#include "token.h"

using namespace std;

class Lexer
{
public:
    Lexer(const string& source);

    vector<Token> scan();

private:
    string source;
    vector<Token> tokens;

    map<string, TokenType> keywords =
    {
        {"when", WHEN},
        {"elif", ELIF},
        {"other", OTHER},
        {"jabtak", JABTAK},
        {"aur", AND},
        {"ya", OR},
        {"for", FOR},
        {"true", TRUE},
        {"false", FALSE},
        {"var", VAR},
        {"scan", SCAN},
        {"show", SHOW},
    };

    int start = 0;
    int curr = 0;
    int line = 1;

    bool atEnd();

    void scanToken();

    bool isdigit(char c);
    bool isalpha(char c);
    bool isalnum(char c);

    char consume();
    char peek();
    char peek2();

    void str();
    void number();
    void identifier();
};