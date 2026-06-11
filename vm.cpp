#include "vm.hpp"

VM vm;

InterpretResult run()
{
    for (;;)
    {
        uint8_t instruction = *vm.ip++;

        switch (instruction)
        {
        case OP_RETURN:
            return INTERPRET_OK;

        default:
            return INTERPRET_RUNTIME_ERROR;
        }
    }
}

void initVM() {}
void freeVM() {}
InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run(); // make this use ast
}