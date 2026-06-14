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
class UnaryExpr;
class PrintStmt;
class VarStmt;
class AssignStmt;
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
    virtual void visitUnary(UnaryExpr& e) = 0;
};

class StmtVisitor
{
public:
    virtual void visitShow(ShowStmt &s) = 0;
    virtual void visitVar(VarStmt &s) = 0;
    virtual void visitAssign(AssignStmt &s) = 0;
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

class UnaryExpr : public Expr
{
public:
    string op;
    unique_ptr<Expr> right;

    UnaryExpr(
        string op,
        unique_ptr<Expr> right)
    {
        this->op = op;
        this->right = move(right);
    }

    void accept(ExpVisitor& v) override
    {
        v.visitUnary(*this);
    }
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

class AssignStmt : public Stmt
{
public:
    string name;
    unique_ptr<Expr> value;

    AssignStmt(
        string name,
        unique_ptr<Expr> value)
    {
        this->name = name;
        this->value = move(value);
    }

    void accept(StmtVisitor& v) override
    {
        v.visitAssign(*this);
    }
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

#include <iostream>

class ASTPrinter : public ExpVisitor, public StmtVisitor
{
    int indent = 0;

    void tabs()
    {
        for (int i = 0; i < indent; i++)
            std::cout << "  ";
    }

public:

    void visitNumber(NumberExpr& e) override
    {
        tabs();
        std::cout << "Number(" << e.value << ")\n";
    }

    void visitUnary(UnaryExpr& e) override
    {
        cout << "(Unary " << e.op << " ";
        e.right->accept(*this);
        cout << ")";
    }

    void visitString(StringExpr& e) override
    {
        tabs();
        std::cout << "String(" << e.value << ")\n";
    }

    void visitBoolean(BooleanExpr& e) override
    {
        tabs();
        std::cout << "Bool(" << (e.value ? "true" : "false") << ")\n";
    }

    void visitVariable(VariableExpr& e) override
    {
        tabs();
        std::cout << "Variable(" << e.name << ")\n";
    }

    void visitBinary(BinaryExpr& e) override
    {
        tabs();
        std::cout << "Binary(" << e.op << ")\n";

        indent++;
        e.left->accept(*this);
        e.right->accept(*this);
        indent--;
    }

    void visitAO(AOExpr& e) override
    {
        tabs();
        std::cout << "Logical(" << e.op << ")\n";

        indent++;
        e.left->accept(*this);
        e.right->accept(*this);
        indent--;
    }

    void visitVar(VarStmt& s) override
    {
        tabs();
        std::cout << "VarStmt(" << s.name << ")\n";

        if (s.initializer)
        {
            indent++;
            s.initializer->accept(*this);
            indent--;
        }
    }

    void visitAssign(AssignStmt& s) override
    {
        tabs();
        std::cout << "AssignStmt(" << s.name << ")\n";

        indent++;
        s.value->accept(*this);
        indent--;
    }

    void visitShow(ShowStmt& s) override
    {
        tabs();
        std::cout << "ShowStmt\n";

        indent++;
        s.expr->accept(*this);
        indent--;
    }

    void visitPrint(PrintStmt& s) override
    {
        tabs();
        std::cout << "PrintStmt\n";

        indent++;
        s.expression->accept(*this);
        indent--;
    }

    void visitScan(ScanStmt& s) override
    {
        tabs();
        std::cout << "ScanStmt(" << s.variable << ")\n";
    }

    void visitBlock(BlockStmt& s) override
    {
        tabs();
        std::cout << "BlockStmt\n";

        indent++;

        for (auto& stmt : s.statements)
            stmt->accept(*this);

        indent--;
    }

    void visitWhile(WhileStmt& s) override
    {
        tabs();
        std::cout << "WhileStmt\n";

        indent++;

        tabs();
        std::cout << "Condition\n";

        indent++;
        s.condition->accept(*this);
        indent--;

        tabs();
        std::cout << "Body\n";

        indent++;
        s.body->accept(*this);
        indent -= 2;
    }

    void visitIf(IfStmt& s) override
    {
        tabs();
        std::cout << "IfStmt\n";

        indent++;

        for (size_t i = 0; i < s.conditions.size(); i++)
        {
            tabs();
            std::cout << "Condition\n";

            indent++;
            s.conditions[i]->accept(*this);
            indent--;

            tabs();
            std::cout << "Then\n";

            indent++;
            s.thenBranches[i]->accept(*this);
            indent--;
        }

        if (s.elseBranch)
        {
            tabs();
            std::cout << "Else\n";

            indent++;
            s.elseBranch->accept(*this);
            indent--;
        }

        indent--;
    }
};