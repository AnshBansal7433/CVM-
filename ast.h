#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

// EXPRESSIONS

class Expr {
public:
    virtual ~Expr() = default;
};

class NumberExpr : public Expr {
public:
    string value;

    NumberExpr(string value) {
        this->value = value;
    }
};

class VariableExpr : public Expr {
public:
    string name;

    VariableExpr(string name) {
        this->name = name;
    }
};

class BinaryExpr : public Expr {
public:
    unique_ptr<Expr> left;
    string op;
    unique_ptr<Expr> right;

    BinaryExpr(
        unique_ptr<Expr> left,
        string op,
        unique_ptr<Expr> right
    ) {
        this->left = move(left);
        this->op = op;
        this->right = move(right);
    }
};

class AOExpr : public Expr {
public:
    unique_ptr<Expr> left;
    string op;
    unique_ptr<Expr> right;

    AOExpr(
        unique_ptr<Expr> left,
        string op,
        unique_ptr<Expr> right
    ){
        this->left = move(left);
        this->op = op;
        this->right = move(right);
    }
};

class BooleanExpr : public Expr {
public:

    bool value;

    BooleanExpr(bool value) {
        this->value = value;
    }
};

// STATEMENTS

class Stmt {
public:
    virtual ~Stmt() = default;
};

class PrintStmt : public Stmt {
public:
    unique_ptr<Expr> expression;

    PrintStmt(unique_ptr<Expr> expression) {
        this->expression = move(expression);
    }
};

class VarStmt : public Stmt {
public:
    string name;
    unique_ptr<Expr> initializer;

    VarStmt(
        string name,
        unique_ptr<Expr> initializer
    ) {
        this->name = name;
        this->initializer = move(initializer);
    }
};

class BlockStmt : public Stmt {
public:
    vector<unique_ptr<Stmt>> statements;

    BlockStmt(vector<unique_ptr<Stmt>> stmts) {
        this->statements = move(stmts);
    }
};

class WhileStmt : public Stmt {
public:
    unique_ptr<Expr> condition;
    unique_ptr<BlockStmt> body;

    WhileStmt(unique_ptr<Expr> condition, unique_ptr<BlockStmt> body) {
        this->condition = move(condition);
        this->body = move(body);
    }
};

class IfStmt : public Stmt {
public:
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> thenBranch;
    unique_ptr<Stmt> elseBranch;

    IfStmt(unique_ptr<Expr> condition,
        unique_ptr<Stmt> thenBranch,
        unique_ptr<Stmt> elseBranch) {
        this->condition = move(condition);
        this->thenBranch = move(thenBranch);
        this->elseBranch = move(elseBranch);
    }
};

// PROGRAM ROOT

class Program {
public:
    vector<unique_ptr<Stmt>> statements;
};