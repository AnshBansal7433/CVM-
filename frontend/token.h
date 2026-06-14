#pragma once

#include <string>

using namespace std;

enum TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    NOT,
    NOT_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    IDENTIFIER,
    STRING,
    NUMBER,

    FOR,
    AND,
    OR,
    JABTAK,
    WHEN,
    ELIF,
    OTHER,
    SCAN,
    SHOW,

    TRUE,
    FALSE,
    VAR,
    TOKEN_EOF,
};

class Token
{
public:
    TokenType type;
    string lexeme;
    int line;

    Token(TokenType type, const string &lexeme, int line)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->line = line;
    }

    string toString()
    {
        return "type: " + to_string(type) + ", value: " + lexeme + ", line: " + to_string(line);
    }
};
