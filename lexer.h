#include <string>
#include <vector>
#include <map>
#include <cctype> 

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
    AGAR,
    MAGAR,
    
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

class Lexer
{
public:
    string source;
    vector<Token> tokens;

    Lexer(const string &source)
    {
        this->source = source;
    }

    vector<Token> scan()
    {
        while (!atEnd())
        {
            start = curr;
            scanToken();
        }

        tokens.push_back(Token(TOKEN_EOF, "", line));
        return tokens;
    }

private:
    // Defined keywords map here with correct type spelling and trailing semicolon
    map<string, TokenType> keywords = {
        {"agar", AGAR},
        {"magar", MAGAR},
        {"jabtak", JABTAK},
        {"aur", AND},
        {"ya", OR},
        {"for", FOR},
        {"true", TRUE},
        {"false", FALSE},
        {"var", VAR},
    };

    int start = 0;
    int curr = 0;
    int line = 1;

    bool atEnd()
    {
        return curr >= source.length();
    }

    void scanToken()
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

        // two char ops
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

        // other stuff
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;

        // strings
        case '"':
            str();
            break;

        default:
            if (isdigit(c))
                number();
            else if (isalpha(c))
                identifier();
            // error handling
        }
    }

    bool isdigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool isalpha(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool isalnum(char c)
    {
        return isalpha(c) || isdigit(c);
    }

    char consume()
    {
        return source[curr++];
    }

    char peek()
    {
        if (atEnd())
            return '\0';
        return source[curr];
    }

    char peek2()
    {
        if (curr + 1 >= source.length())
            return '\0';
        return source[curr + 1];
    }

    void str()
    {
        while (peek() != '"' && !atEnd())
        {
            consume();
        }
        consume();
        string value = source.substr(start + 1, curr - start - 2);
        tokens.push_back(Token(STRING, value, line));
    }

    void number()
    {
        while (isdigit(peek()))
            consume();
        if (peek() == '.' && isdigit(peek2()))
        {
            consume();
            while (isdigit(peek()))
                consume();
        }

        string value = source.substr(start, curr - start);
        tokens.push_back(Token(NUMBER, value, line));
    }

    void identifier()
    {
        while (isalnum(peek()))
            consume();
        string text = source.substr(start, curr - start);
        
        // Fixed C++ map search mechanics
        TokenType type;
        if (keywords.find(text) != keywords.end())
        {
            type = keywords[text];
        }
        else
        {
            type = IDENTIFIER;
        }
        
        tokens.push_back(Token(type, text, line));
    }
};