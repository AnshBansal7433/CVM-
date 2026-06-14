#pragma once
#include "../frontend/ast.h"
#include "../vm/chunk.h"

class Compiler : public ExpVisitor, public StmtVisitor
{
private:
    Chunk *chunk;

    void emitByte(uint8_t byte) { writeChunk(chunk, byte); }
    void emitBytes(uint8_t b1, uint8_t b2)
    {
        emitByte(b1);
        emitByte(b2);
    }

    uint8_t makeConstant(Value val)
    {
        return (uint8_t)addConstant(chunk, val);
    }

    uint8_t resolveName(const string& name)
    {
        for (size_t i = 0; i < chunk->names.size(); i++)
        {
            if (chunk->names[i] == name)
                return (uint8_t)i;
        }

        chunk->names.push_back(name);
        return (uint8_t)(chunk->names.size() - 1);
    }

    int emitJump(uint8_t instruction)
    {
        emitByte(instruction);
        emitByte(0xff);
        emitByte(0xff);
        return chunk->count - 2;
    }

    void patchJump(int offset)
    {
        int jump = chunk->count - offset - 2;
        chunk->code[offset] = (jump >> 8) & 0xff;
        chunk->code[offset + 1] = jump & 0xff;
    }

    void emitLoop(int loopStart)
    {
        emitByte(OP_LOOP);
        int offset = chunk->count - loopStart + 2;
        emitByte((offset >> 8) & 0xff);
        emitByte(offset & 0xff);
    }

public:
    Compiler(Chunk *chunk) : chunk(chunk) {}

    void compile(Program &program)
    {
        for (auto &stmt : program.statements)
            stmt->accept(*this);
        emitByte(OP_RETURN);
    }

    // EXPRESSIONS

    void visitNumber(NumberExpr &e) override
    {
        emitBytes(OP_CONSTANT, makeConstant(NUMBER_VAL(stod(e.value))));
    }

    void visitBoolean(BooleanExpr &e) override
    {
        emitByte(e.value ? OP_TRUE : OP_FALSE);
    }

    void visitVariable(VariableExpr &e) override
    {
        emitBytes(OP_LOAD, resolveName(e.name));
    }

    void visitString(StringExpr &e) override
    {
        auto* str = new std::string(e.value);

        emitBytes(
            OP_CONSTANT,
            makeConstant(STRING_VAL(str))
        );
    }

    void visitBinary(BinaryExpr &e) override
    {
        e.left->accept(*this);
        e.right->accept(*this);
        if (e.op == "+")
            emitByte(OP_ADD);
        else if (e.op == "-")
            emitByte(OP_SUB);
        else if (e.op == "*")
            emitByte(OP_MUL);
        else if (e.op == "/")
            emitByte(OP_DIV);
        else if (e.op == "==")
            emitByte(OP_EQUAL);
        else if (e.op == "!=")
            emitByte(OP_NOT_EQUAL);
        else if (e.op == "<")
            emitByte(OP_LESS);
        else if (e.op == "<=")
            emitByte(OP_LESS_EQUAL);
        else if (e.op == ">")
            emitByte(OP_GREATER);
        else if (e.op == ">=")
            emitByte(OP_GREATER_EQUAL);
    }

    void visitAO(AOExpr &e) override
    {
        e.left->accept(*this);
        e.right->accept(*this);
        if (e.op == "aur")
            emitByte(OP_AND);
        else if (e.op == "ya")
            emitByte(OP_OR);
    }

    // STATEMENTS

    void visitShow(ShowStmt &s) override
    {
        s.expr->accept(*this);
        emitByte(OP_PRINT);
        emitByte(OP_POP);
    }

    void visitPrint(PrintStmt &s) override
    {
        s.expression->accept(*this);
        emitByte(OP_PRINT);
        emitByte(OP_POP);
    }

    void visitVar(VarStmt &s) override
    {
        if (s.initializer)
            s.initializer->accept(*this);
        else
            emitBytes(OP_CONSTANT, makeConstant(NUMBER_VAL(0)));
        emitBytes(OP_STORE, resolveName(s.name));
    }

    void visitAssign(AssignStmt &s) override
    {
        s.value->accept(*this);
        emitBytes(
            OP_STORE,
            resolveName(s.name));
    }

    void visitBlock(BlockStmt &s) override
    {
        for (auto &stmt : s.statements)
            stmt->accept(*this);
    }

    void visitIf(IfStmt &s) override
    {
        vector<int> endJumps;
        for (int i = 0; i < (int)s.conditions.size(); i++)
        {
            s.conditions[i]->accept(*this);
            int jumpToNext = emitJump(OP_JUMP_IF_FALSE);
            emitByte(OP_POP);
            s.thenBranches[i]->accept(*this);
            endJumps.push_back(emitJump(OP_JUMP));
            patchJump(jumpToNext);
            emitByte(OP_POP);
        }
        if (s.elseBranch)
            s.elseBranch->accept(*this);
        for (int jump : endJumps)
            patchJump(jump);
    }

    void visitWhile(WhileStmt &s) override
    {
        int loopStart = chunk->count;
        s.condition->accept(*this);
        int exitJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP);
        s.body->accept(*this);
        emitLoop(loopStart);
        patchJump(exitJump);
        emitByte(OP_POP);
    }

    void visitScan(ScanStmt &s) override
    {
        emitBytes(OP_SCAN, resolveName(s.variable));
    }
};