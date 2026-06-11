#pragma once
#include "lexer.h"
#include "ast.h"

class Parser
{

private:
    vector<Token> tokens;

    int curr = 0;

    bool atEnd()
    {
        return peek().type == TOKEN_EOF;
    }

    Token peek2()
    {
        if (curr + 1 >= tokens.size())
        {
            return peek();
        }
        return tokens[curr + 1];
    }

    Token peek()
    {
        return tokens[curr];
    }

    Token previous()
    {
        return tokens[curr - 1];
    }

    void back()
    {
        curr--;
    }

    Token advance()
    {
        if (!atEnd())
            curr++;
        return previous();
    }

    bool check(TokenType type)
    {
        if (atEnd())
            return false;
        return peek().type == type;
    }

    bool match(initializer_list<TokenType> types)
    {
        for (TokenType type : types)
        {
            if (check(type))
            {
                advance();
                return true;
            }
        }
        return false;
    }

public:
    Parser(vector<Token> tokens)
    {
        this->tokens = tokens;
    }

    Program parse()
    {
        Program program;
        while (!atEnd())
        {
            program.statements.push_back(statement());
        }
        return program;
    }

    unique_ptr<Stmt> statement()
    {
        if (match({VAR}))
            return varDeclaration();
        else if (match({SHOW}))
            return showStatement();
        else if (match({SCAN}))
            return scanStatement();
        else if (check(IDENTIFIER) && peek2().type == EQUAL){
            advance();
            return varChange();
        }
        else if (match({JABTAK}))
            return whileStatement();
        else if (match({FOR}))
            return forStatement();
        else if (match({WHEN}))
            return ifStatement();
        return printStatement();
    }

    // STATEMENTS

    unique_ptr<Stmt> varDeclaration()
    {
        Token name = advance();
        unique_ptr<Expr> init = nullptr;
        if (!atEnd() && peek().type == EQUAL)
        {
            advance();
            init = expression();
        }
        match({SEMICOLON});
        return make_unique<VarStmt>(name.lexeme, move(init));
    }

    unique_ptr<Stmt> varChange()
    {
        Token name = previous();
        if (match({EQUAL}))
        {
            auto init = expression();
            match({SEMICOLON});
            return make_unique<AssignStmt>(name.lexeme, move(init));
        }
        match({SEMICOLON});
        return nullptr;
    }

    unique_ptr<Stmt> showStatement()
    {
        auto expr = expression();
        match({SEMICOLON});
        return make_unique<ShowStmt>(
            move(expr)
        );
    }

    unique_ptr<Stmt> scanStatement()
    {
        match({IDENTIFIER});
        string name = previous().lexeme;
        match({SEMICOLON});
        return make_unique<ScanStmt>(name);
    }

    unique_ptr<Stmt> printStatement()
    {
        auto expr = expression();
        match({SEMICOLON});
        return make_unique<PrintStmt>(move(expr));
    }

    // EXPRESSIONS

    unique_ptr<Expr> primary()
    {
        if (match({NUMBER}))
        {
            return make_unique<NumberExpr>(previous().lexeme);
        }

        if (match({STRING}))
        {
            return make_unique<StringExpr>(previous().lexeme);
        }

        if (match({IDENTIFIER}))
        {
            return make_unique<VariableExpr>(previous().lexeme);
        }

        if (match({LEFT_PAREN}))
        {
            auto expr = expression();
            match({RIGHT_PAREN});
            return expr;
        }

        if (match({TRUE}))
        {
            return make_unique<BooleanExpr>(true);
        }

        if (match({FALSE}))
        {
            return make_unique<BooleanExpr>(false);
        }
        return nullptr;
    }

    unique_ptr<Expr> AndOr()
    {
        auto expr = comparison();
        while (match({AND, OR}))
        {
            Token op = previous();
            auto right = comparison();
            expr = make_unique<AOExpr>(
                move(expr),
                op.lexeme,
                move(right));
        }
        return expr;
    }

    unique_ptr<Expr> addition()
    {
        auto expr = multiplication();
        while (match({PLUS, MINUS}))
        {
            Token op = previous();
            auto right = multiplication();
            expr = make_unique<BinaryExpr>(
                move(expr),
                op.lexeme,
                move(right));
        }
        return expr;
    }

    unique_ptr<Expr> multiplication()
    {
        auto expr = primary();
        while (match({STAR, SLASH}))
        {
            Token op = previous();
            auto right = primary();
            expr = make_unique<BinaryExpr>(
                move(expr),
                op.lexeme,
                move(right));
        }
        return expr;
    }

    unique_ptr<Expr> expression()
    {
        return AndOr();
    }

    unique_ptr<Expr> comparison()
    {
        auto expr = addition();
        while (match({LESS,
                      LESS_EQUAL,
                      GREATER,
                      GREATER_EQUAL,
                      EQUAL_EQUAL,
                      NOT_EQUAL}))
        {
            Token op = previous();
            auto right = addition();
            expr = make_unique<BinaryExpr>(
                move(expr),
                op.lexeme,
                move(right));
        }
        return expr;
    }

    unique_ptr<BlockStmt> blockStatement()
    {
        match({LEFT_BRACE});
        vector<unique_ptr<Stmt>> stmts;
        while (!check(RIGHT_BRACE)&& !atEnd())
        {
            stmts.push_back(statement());
        }
        match({RIGHT_BRACE});
        return make_unique<BlockStmt>(move(stmts));
    }

    unique_ptr<Stmt> whileStatement()
    {
        match({LEFT_PAREN});
        auto condition = expression();
        match({RIGHT_PAREN});
        auto body = blockStatement();
        return make_unique<WhileStmt>(move(condition), move(body));
    }

    unique_ptr<Stmt> forStatement()
    {
        match({LEFT_PAREN});
        unique_ptr<Stmt> initializer = nullptr;
        if (match({VAR}))
        {
            initializer = varDeclaration();
        }
        else if (match({IDENTIFIER}))
        {
            initializer = varChange();
        }
        unique_ptr<Expr> condition = expression();
        match({SEMICOLON});
        unique_ptr<Stmt> increment = nullptr;
        if (match({IDENTIFIER}))
        {
            increment = varChange();
        }
        match({RIGHT_PAREN});
        auto body = blockStatement();
        if (increment != nullptr)
        {
            body->statements.push_back(move(increment));
        }
        auto whileLoop = make_unique<WhileStmt>(move(condition), move(body));
        vector<unique_ptr<Stmt>> FStmts;
        if (initializer != nullptr)
        {
            FStmts.push_back(move(initializer));
        }
        FStmts.push_back(move(whileLoop));
        return make_unique<BlockStmt>(move(FStmts));
    }

    unique_ptr<Stmt> ifStatement()
    {
        vector<unique_ptr<Expr>> conditions;
        vector<unique_ptr<Stmt>> thenBranches;
        match({LEFT_PAREN});
        auto condition = expression();
        conditions.push_back(std::move(condition));
        match({RIGHT_PAREN});
        auto thenBranch = blockStatement();
        thenBranches.push_back(std::move(thenBranch));
        unique_ptr<Stmt> elseBranch = nullptr;
        while(match({ELIF})){
            match({LEFT_PAREN});
            auto condition = expression();
            conditions.push_back(std::move(condition));
            match({RIGHT_PAREN});
            auto thenBranch = blockStatement();
            thenBranches.push_back(std::move(thenBranch));
        }
        if (match({OTHER}))
        {
            if (check(LEFT_BRACE))
            {
                elseBranch = blockStatement();
            }
        }

        return make_unique<IfStmt>(
            move(conditions),
            move(thenBranches),
            move(elseBranch));
    }
};