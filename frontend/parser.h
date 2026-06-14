#pragma once

#include "lexer.h"
#include "ast.h"

class Parser
{
private:
    vector<Token> tokens;
    int curr = 0;

    bool atEnd();
    Token peek2();
    Token peek();
    Token previous();
    void back();
    Token advance();
    bool check(TokenType type);
    bool match(initializer_list<TokenType> types);

public:
    Parser(vector<Token> tokens);

    Program parse();

    unique_ptr<Stmt> statement();

    unique_ptr<Stmt> varDeclaration();
    unique_ptr<Stmt> varChange();
    unique_ptr<Stmt> showStatement();
    unique_ptr<Stmt> scanStatement();
    unique_ptr<Stmt> printStatement();

    unique_ptr<Expr> primary();
    unique_ptr<Expr> AndOr();
    unique_ptr<Expr> addition();
    unique_ptr<Expr> multiplication();
    unique_ptr<Expr> expression();
    unique_ptr<Expr> comparison();
    unique_ptr<Expr> unary();

    unique_ptr<BlockStmt> blockStatement();
    unique_ptr<Stmt> whileStatement();
    unique_ptr<Stmt> forStatement();
    unique_ptr<Stmt> ifStatement();
};