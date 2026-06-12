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
    uint8_t *code;
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
static int byteInstr(const char *name, Chunk *chunk, int offset);           // for OP_LOAD/OP_STORE/OP_SCAN (1-byte operand)
static int jumpInstr(const char *name, int sign, Chunk *chunk, int offset); // for jumps (2-byte operand)
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
    case OP_CONSTANT:
        return constantInstr("OP_CONSTANT", chunk, offset);
    case OP_TRUE:
        return simpleInstr("OP_TRUE", offset);
    case OP_FALSE:
        return simpleInstr("OP_FALSE", offset);
    case OP_POP:
        return simpleInstr("OP_POP", offset);
    case OP_ADD:
        return simpleInstr("OP_ADD", offset);
    case OP_SUB:
        return simpleInstr("OP_SUB", offset);
    case OP_MUL:
        return simpleInstr("OP_MUL", offset);
    case OP_DIV:
        return simpleInstr("OP_DIV", offset);
    case OP_EQUAL:
        return simpleInstr("OP_EQUAL", offset);
    case OP_NOT_EQUAL:
        return simpleInstr("OP_NOT_EQUAL", offset);
    case OP_LESS:
        return simpleInstr("OP_LESS", offset);
    case OP_LESS_EQUAL:
        return simpleInstr("OP_LESS_EQUAL", offset);
    case OP_GREATER:
        return simpleInstr("OP_GREATER", offset);
    case OP_GREATER_EQUAL:
        return simpleInstr("OP_GREATER_EQUAL", offset);
    case OP_AND:
        return simpleInstr("OP_AND", offset);
    case OP_OR:
        return simpleInstr("OP_OR", offset);
    case OP_NEGATE:
        return simpleInstr("OP_NEGATE", offset);
    case OP_NOT:
        return simpleInstr("OP_NOT", offset);
    case OP_LOAD:
        return byteInstr("OP_LOAD", chunk, offset);
    case OP_STORE:
        return byteInstr("OP_STORE", chunk, offset);
    case OP_JUMP:
        return jumpInstr("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
        return jumpInstr("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
        return jumpInstr("OP_LOOP", -1, chunk, offset);
    case OP_PRINT:
        return simpleInstr("OP_PRINT", offset);
    case OP_SCAN:
        return byteInstr("OP_SCAN", chunk, offset);
    case OP_RETURN:
        return simpleInstr("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
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
    printf("%-16s %4d ", name, constant);
    if (constant >= chunk->constants.count)
    {
        printf("Invalid constant index %d\n", constant);
        return chunk->count;
    }
    printValue(chunk->constants.values[constant]);
    printf("\n");
    return offset + 2;
}

// for OP_LOAD / OP_STORE / OP_SCAN — operand is an index into chunk->names
static int byteInstr(const char *name, Chunk *chunk, int offset)
{
    uint8_t slot = chunk->code[offset + 1];
    printf("%-16s %4d", name, slot);
    if (slot < chunk->names.size())
        printf(" '%s'", chunk->names[slot].c_str());
    printf("\n");
    return offset + 2;
}

// for OP_JUMP / OP_JUMP_IF_FALSE / OP_LOOP — 2-byte big-endian operand
static int jumpInstr(const char *name, int sign, Chunk *chunk, int offset)
{
    uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    int target = offset + 3 + sign * jump;
    printf("%-16s %4d -> %d\n", name, offset, target);
    return offset + 3;
}

inline int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}