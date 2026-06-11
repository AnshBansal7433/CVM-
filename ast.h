#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

class NumberExpr;
class VariableExpr;
class StringExpr;
class BinaryExpr;
class AOExpr;
class BooleanExpr;
class PrintStmt;
class VarStmt;
class BlockStmt;
class WhileStmt;
class IfStmt;
class ShowStmt;
class ScanStmt;

class ExpVisitor
{
public:
    virtual void visitNumber(NumberExpr &e) = 0;
    virtual void visitString(StringExpr &e) = 0;
    virtual void visitBoolean(BooleanExpr &e) = 0;
    virtual void visitVariable(VariableExpr &e) = 0;
    virtual void visitBinary(BinaryExpr &e) = 0;
    virtual void visitAO(AOExpr &e) = 0;
};

class StmtVisitor
{
public:
    virtual void visitShow(ShowStmt &s) = 0;
    virtual void visitVar(VarStmt &s) = 0;
    virtual void visitBlock(BlockStmt &s) = 0;
    virtual void visitIf(IfStmt &s) = 0;
    virtual void visitWhile(WhileStmt &s) = 0;
    virtual void visitScan(ScanStmt &s) = 0;
    virtual void visitPrint(PrintStmt &s) = 0;
};

// EXPRESSIONS

class Expr
{
public:
    virtual ~Expr() = default;
    virtual void accept(ExpVisitor &v) = 0;
};

class NumberExpr : public Expr
{
public:
    string value;

    NumberExpr(string value)
    {
        this->value = value;
    }
    void accept(ExpVisitor &v) override { v.visitNumber(*this); }
};

class VariableExpr : public Expr
{
public:
    string name;

    VariableExpr(string name)
    {
        this->name = name;
    }
    void accept(ExpVisitor &v) override { v.visitVariable(*this); }
};

class StringExpr : public Expr
{
public:
    string value;

    StringExpr(string value)
    {
        this->value = value;
    }
    void accept(ExpVisitor &v) override { v.visitString(*this); }
};
class BinaryExpr : public Expr
{
public:
    unique_ptr<Expr> left;
    string op;
    unique_ptr<Expr> right;

    BinaryExpr(
        unique_ptr<Expr> left,
        string op,
        unique_ptr<Expr> right)
    {
        this->left = move(left);
        this->op = op;
        this->right = move(right);
    }
    void accept(ExpVisitor &v) override { v.visitBinary(*this); }
};

class AOExpr : public Expr
{
public:
    unique_ptr<Expr> left;
    string op;
    unique_ptr<Expr> right;

    AOExpr(
        unique_ptr<Expr> left,
        string op,
        unique_ptr<Expr> right)
    {
        this->left = move(left);
        this->op = op;
        this->right = move(right);
    }
    void accept(ExpVisitor &v) override { v.visitAO(*this); }
};

class BooleanExpr : public Expr
{
public:
    bool value;

    BooleanExpr(bool value)
    {
        this->value = value;
    }
    void accept(ExpVisitor &v) override { v.visitBoolean(*this); }
};

// STATEMENTS

class Stmt
{
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &v) = 0;
};

class PrintStmt : public Stmt
{
public:
    unique_ptr<Expr> expression;

    PrintStmt(unique_ptr<Expr> expression)
    {
        this->expression = move(expression);
    }
    void accept(StmtVisitor &v) override { v.visitPrint(*this); }
};

class VarStmt : public Stmt
{
public:
    string name;
    unique_ptr<Expr> initializer;

    VarStmt(
        string name,
        unique_ptr<Expr> initializer)
    {
        this->name = name;
        this->initializer = move(initializer);
    }
    void accept(StmtVisitor &v) override { v.visitVar(*this); }
};

class BlockStmt : public Stmt
{
public:
    vector<unique_ptr<Stmt>> statements;

    BlockStmt(vector<unique_ptr<Stmt>> stmts)
    {
        this->statements = move(stmts);
    }
    void accept(StmtVisitor &v) override { v.visitBlock(*this); }
};

class WhileStmt : public Stmt
{
public:
    unique_ptr<Expr> condition;
    unique_ptr<BlockStmt> body;

    WhileStmt(unique_ptr<Expr> condition, unique_ptr<BlockStmt> body)
    {
        this->condition = move(condition);
        this->body = move(body);
    }
    void accept(StmtVisitor &v) override { v.visitWhile(*this); }
};

class IfStmt : public Stmt
{
public:
    vector<unique_ptr<Expr>> conditions;
    vector<unique_ptr<Stmt>> thenBranches;
    unique_ptr<Stmt> elseBranch;

    IfStmt(vector<unique_ptr<Expr>> conditions,
           vector<unique_ptr<Stmt>> thenBranches,
           unique_ptr<Stmt> elseBranch)
    {
        this->conditions = move(conditions);
        this->thenBranches = move(thenBranches);
        this->elseBranch = move(elseBranch);
    }
    void accept(StmtVisitor &v) override { v.visitIf(*this); }
};

class ShowStmt : public Stmt
{
public:
    unique_ptr<Expr> expr;

    ShowStmt(unique_ptr<Expr> expr)
    {
        this->expr = move(expr);
    }
    void accept(StmtVisitor &v) override { v.visitShow(*this); }
};

class ScanStmt : public Stmt
{
public:
    string variable;

    ScanStmt(string variable)
    {
        this->variable = variable;
    }
    void accept(StmtVisitor &v) override { v.visitScan(*this); }
};

// PROGRAM ROOT

class Program
{
public:
    vector<unique_ptr<Stmt>> statements;
};