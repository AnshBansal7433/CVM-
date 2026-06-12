#pragma once

#include <vector>
#include <iostream>
#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef enum
{
    INTERPRET_OK,
} InterpretResult;

class VM
{
private:
    Chunk *chunk;
    uint8_t *ip;
    Value stack[STACK_MAX];
    Value *stackTop;
    std::vector<Value> globals;

    void resetStack()
    {
        stackTop = stack;
    }

    void push(Value value)
    {
        *stackTop = value;
        stackTop++;
    }

    Value pop()
    {
        stackTop--;
        return *stackTop;
    }

    Value peek(int distance)
    {
        return stackTop[-1 - distance];
    }

    uint8_t readByte()
    {
        return *ip++;
    }

    uint16_t readShort()
    {
        uint16_t val = (uint16_t)((ip[0] << 8) | ip[1]);
        ip += 2;
        return val;
    }

    Value readConstant()
    {
        return chunk->constants.values[readByte()];
    }

    bool isFalsey(Value value)
    {
        if (IS_BOOL(value))
            return !AS_BOOL(value);
        if (IS_NUMBER(value))
            return AS_NUMBER(value) == 0;
        return false;
    }

    void ensureGlobalSlot(size_t index)
    {
        if (globals.size() <= index)
            globals.resize(index + 1, NUMBER_VAL(0));
    }

    InterpretResult run();

public:
    VM()
    {
        resetStack();
    }

    InterpretResult interpret(Chunk *chunk)
    {
        this->chunk = chunk;
        this->ip = chunk->code;
        globals.resize(chunk->names.size(), NUMBER_VAL(0));
        return run();
    }
};