#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>

#include "value.h"

typedef enum
{
    OP_CONSTANT,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_AND,
    OP_OR,
    OP_NEGATE,
    OP_NOT,
    OP_LOAD,
    OP_STORE,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_PRINT,
    OP_SCAN,
    OP_RETURN,
} OpCode;

struct Chunk
{
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
    std::vector<std::string> names;
};

inline void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = nullptr;
    initValueArray(&chunk->constants);
}

inline void writeChunk(Chunk *chunk, uint8_t byte)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

inline void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

static int simpleInstr(const char *name, int offset);
static int constantInstr(const char *name, Chunk *chunk, int offset);
int disassembleInstr(Chunk *chunk, int offset);

inline void disassembleChunk(Chunk *chunk, const char *name)
{
    printf("%s", name);
    for (int offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstr(chunk, offset);
    }
}

inline int disassembleInstr(Chunk *chunk, int offset)
{
    printf("%04d ", offset);
    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstr("OP_RETURN", offset);
    case OP_CONSTANT:
        return constantInstr("OP_CONSTANT", chunk, offset);
    default:
        return offset + 1;
    }
}

static int simpleInstr(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstr(const char *name, Chunk *chunk, int offset)
{
    if (offset + 1 >= chunk->count)
    {
        printf("Corrupt bytecode\n");
        return chunk->count;
    }
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d", name, constant);
    if (constant >= chunk->constants.count)
    {
        printf("Invalid constant index %d\n", constant);
        return chunk->count;
    }
    printValue(chunk->constants.values[constant]);
    printf("\n");
    return offset + 2;
}

inline int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}