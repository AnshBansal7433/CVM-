#include "vm.hpp"

VM vm;

void initVM() {}
void freeVM() {}
InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run(); // make this use ast
}