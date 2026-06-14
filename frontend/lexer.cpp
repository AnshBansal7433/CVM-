#include "lexer.h"

Lexer::Lexer(const string& source)
{
    this->source = source;
}

vector<Token> Lexer::scan()
{
    while (!atEnd())
    {
        start = curr;
        scanToken();
    }

    tokens.push_back(Token(TOKEN_EOF, "", line));
    return tokens;
}

bool Lexer::atEnd()
{
    return curr >= source.length();
}

char Lexer::consume()
{
    return source[curr++];
}

char Lexer::peek()
{
    if (atEnd())
        return '\0';
    return source[curr];
}

char Lexer::peek2()
{
    if (curr + 1 >= source.length())
        return '\0';
    return source[curr + 1];
}

bool Lexer::isdigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Lexer::isalpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isalnum(char c)
{
    return isalpha(c) || isdigit(c);
}

void Lexer::scanToken()
{
    char c = consume();

    switch (c)
    {
    case '(':
        tokens.push_back(Token(LEFT_PAREN, "(", line));
        break;

    case ')':
        tokens.push_back(Token(RIGHT_PAREN, ")", line));
        break;

    case '{':
        tokens.push_back(Token(LEFT_BRACE, "{", line));
        break;

    case '}':
        tokens.push_back(Token(RIGHT_BRACE, "}", line));
        break;

    case ',':
        tokens.push_back(Token(COMMA, ",", line));
        break;

    case '.':
        tokens.push_back(Token(DOT, ".", line));
        break;

    case '-':
        tokens.push_back(Token(MINUS, "-", line));
        break;

    case '+':
        tokens.push_back(Token(PLUS, "+", line));
        break;

    case ';':
        tokens.push_back(Token(SEMICOLON, ";", line));
        break;

    case '*':
        tokens.push_back(Token(STAR, "*", line));
        break;

    case '/':
        tokens.push_back(Token(SLASH, "/", line));
        break;

    case '!':
        if (peek() == '=')
        {
            consume();
            tokens.push_back(Token(NOT_EQUAL, "!=", line));
        }
        else
        {
            tokens.push_back(Token(NOT, "!", line));
        }
        break;

    case '=':
        if (peek() == '=')
        {
            consume();
            tokens.push_back(Token(EQUAL_EQUAL, "==", line));
        }
        else
        {
            tokens.push_back(Token(EQUAL, "=", line));
        }
        break;

    case '<':
        if (peek() == '=')
        {
            consume();
            tokens.push_back(Token(LESS_EQUAL, "<=", line));
        }
        else
        {
            tokens.push_back(Token(LESS, "<", line));
        }
        break;

    case '>':
        if (peek() == '=')
        {
            consume();
            tokens.push_back(Token(GREATER_EQUAL, ">=", line));
        }
        else
        {
            tokens.push_back(Token(GREATER, ">", line));
        }
        break;

    case ' ':
    case '\r':
    case '\t':
        break;

    case '\n':
        line++;
        break;

    case '"':
        str();
        break;

    default:
        if (isdigit(c))
        {
            number();
        }
        else if (isalpha(c))
        {
            identifier();
        }
        break;
    }
}

void Lexer::str()
{
    while (peek() != '"' && !atEnd())
    {
        consume();
    }

    consume();

    string value =
        source.substr(start + 1, curr - start - 2);

    tokens.push_back(Token(STRING, value, line));
}

void Lexer::number()
{
    while (isdigit(peek()))
    {
        consume();
    }

    if (peek() == '.' && isdigit(peek2()))
    {
        consume();

        while (isdigit(peek()))
        {
            consume();
        }
    }

    string value =
        source.substr(start, curr - start);

    tokens.push_back(Token(NUMBER, value, line));
}

void Lexer::identifier()
{
    while (isalnum(peek()))
    {
        consume();
    }

    string text =
        source.substr(start, curr - start);

    TokenType type;

    auto it = keywords.find(text);

    if (it != keywords.end())
    {
        type = it->second;
    }
    else
    {
        type = IDENTIFIER;
    }

    tokens.push_back(Token(type, text, line));
}