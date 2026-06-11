#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "compiler.hpp"

int main()
{
    std::string source = "var x = 5; show x + 3;";

    Lexer lexer(source);
    auto tokens = lexer.scan();

    Parser parser(tokens);
    auto program = parser.parse();

    Chunk chunk;
    initChunk(&chunk);

    Compiler compiler(&chunk);
    compiler.compile(program);

    disassembleChunk(&chunk, "=== bytecode ===\n");

    freeChunk(&chunk);
    return 0;
}