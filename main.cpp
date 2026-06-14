#include <iostream>
#include <fstream>
#include <sstream>

#include "frontend/lexer.cpp"
#include "frontend/parser.cpp"
#include "compiler/compiler.h"
#include "vm/vm.cpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: cvm <file>\n";
        return 1;
    }

    ifstream file(argv[1]);

    stringstream buffer;
    buffer << file.rdbuf();

    string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.scan();

    Parser parser(tokens);
    auto program = parser.parse();

    Chunk chunk;
    initChunk(&chunk);

    Compiler compiler(&chunk);
    compiler.compile(program);

    VM vm;
    vm.interpret(&chunk);

    freeChunk(&chunk);
    return 0;
}