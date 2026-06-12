#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "compiler.hpp"
#include "vm.hpp"

int main()
{
    string source = "var x = 5; show x + 3;";

    Lexer lexer(source);
    auto tokens = lexer.scan();

    Parser parser(tokens);
    auto program = parser.parse();

    Chunk chunk;
    initChunk(&chunk);

    Compiler compiler(&chunk);
    compiler.compile(program);

    disassembleChunk(&chunk, "=== bytecode ===\n");

    printf("=== output ===\n");
    VM vm;
    vm.interpret(&chunk);

    freeChunk(&chunk);
    return 0;
}