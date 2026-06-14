# CVM - Custom Virtual Machine

A small interpreted programming language written in **C++** featuring:

- Lexer
- Parser
- Abstract Syntax Tree (AST)
- Bytecode Compiler
- Stack-Based Virtual Machine

The project demonstrates the complete language pipeline from source code to execution.

---

# Features

- Variable declarations
- Variable assignment
- Arithmetic expressions
- Boolean expressions
- Comparison operators
- Logical operators
- If / Else If / Else
- While loops
- For loops
- User input
- Output statements
- Bytecode compilation
- Stack-based VM execution

---

# Language Syntax

## Variables

```c
var x = 10;
var y = 20;
```

## Assignment

```c
x = x + 5;
```

## Arithmetic

```c
var result = (3 + 5) * (6 + 7);

show result;
```

Output:

```text
104
```

---

## Boolean Values

```c
var a = true;
var b = false;
```

---

## Comparison Operators

```c
show 10 > 5;
show 10 < 5;
show 10 >= 5;
show 10 <= 5;
show 10 == 5;
show 10 != 5;
```

---

## Logical Operators

| Operator | Meaning |
|-----------|----------|
| aur | AND |
| ya | OR |

Example:

```c
when (x > 0 aur x < 100) {
    show x;
}
```

---

## If Statement

```c
when (x > 10) {
    show x;
}
```

---

## Else If

```c
when (x > 10) {
    show 1;
}
elif (x > 5) {
    show 2;
}
```

---

## Else

```c
when (x > 10) {
    show 1;
}
other {
    show 0;
}
```

---

## While Loop

```c
var i = 0;

jabtak (i < 5) {
    show i;
    i = i + 1;
}
```

Output:

```text
0
1
2
3
4
```

---

## For Loop

```c
for (var i = 0; i < 5; i = i + 1;) {
    show i;
}
```

---

## Input

```c
scan x;
show x;
```

---

# Keywords

| Keyword | Description |
|----------|-------------|
| var | Variable declaration |
| show | Print output |
| scan | Read input |
| when | If statement |
| elif | Else-if statement |
| other | Else statement |
| jabtak | While loop |
| for | For loop |
| aur | Logical AND |
| ya | Logical OR |
| true | Boolean true |
| false | Boolean false |

---

# Example Program

```c
var x=(3+5)*(6+7);

when(x aur 0){
    show x;
}
```

Output:

```text
104
```

---

# Lexer Demo

Source:

```c
var x = 10;
```

Output:

```text
type: VAR, value: var, line: 1
type: IDENTIFIER, value: x, line: 1
type: EQUAL, value: =, line: 1
type: NUMBER, value: 10, line: 1
type: SEMICOLON, value: ;, line: 1
type: TOKEN_EOF, value: , line: 1
```

---

# Lexer Demo - Conditional Statement

Source:

```c
when (x >= 10) {
    x = x + 1;
}
```

Output:

```text
type: WHEN, value: when, line: 1
type: LEFT_PAREN, value: (, line: 1
type: IDENTIFIER, value: x, line: 1
type: GREATER_EQUAL, value: >=, line: 1
type: NUMBER, value: 10, line: 1
type: RIGHT_PAREN, value: ), line: 1
type: LEFT_BRACE, value: {, line: 1
type: IDENTIFIER, value: x, line: 2
type: EQUAL, value: =, line: 2
type: IDENTIFIER, value: x, line: 2
type: PLUS, value: +, line: 2
type: NUMBER, value: 1, line: 2
type: SEMICOLON, value: ;, line: 2
type: RIGHT_BRACE, value: }, line: 3
type: TOKEN_EOF, value: , line: 3
```

---

# AST Demo

Source:

```c
var x=(3+5)*(6+7);
```

Example AST:

```text
VarStmt(x)
└── BinaryExpr(*)
    ├── BinaryExpr(+)
    │   ├── NumberExpr(3)
    │   └── NumberExpr(5)
    └── BinaryExpr(+)
        ├── NumberExpr(6)
        └── NumberExpr(7)
```

> Replace this section with the exact output produced by your AST printer.

---

# Bytecode Demo

Source:

```c
var x = 10;
show x;
```

Example Bytecode:

```text
0000 OP_CONSTANT      0 10
0002 OP_STORE         0 'x'
0004 OP_LOAD          0 'x'
0006 OP_PRINT
0007 OP_RETURN
```

> Replace this section with the exact output from your disassembler.

---

# Execution Demo

Program:

```c
var a = 10;
var b = 20;

show a + b;
```

Output:

```text
30
```

---

# Architecture

```text
Source Code
    ↓
Lexer
    ↓
Tokens
    ↓
Parser
    ↓
AST
    ↓
Compiler
    ↓
Bytecode
    ↓
Virtual Machine
    ↓
Program Output
```

---

# Project Structure

```text
.
├── main.cpp
├── lexer.h
├── parser.h
├── ast.h
├── compiler.hpp
├── chunk.h
├── value.h
├── vm.hpp
├── vm.cpp
├── hi.txt
├── build.bat
└── README.md
```

---

# Build

Windows:

```bat
build.bat
```

This generates:

```text
cvm.exe
```

---

# Run

```bat
cvm.exe hi.txt
```

Example:

```bat
cvm.exe program.txt
```

---

# Compilation Pipeline

```text
Source Code
      ↓
    Lexer
      ↓
    Tokens
      ↓
    Parser
      ↓
      AST
      ↓
   Compiler
      ↓
   Bytecode
      ↓
Virtual Machine
      ↓
    Output
```


---

# Inspiration

CVM is a learning project built to understand how programming languages work internally—from lexical analysis and parsing to bytecode generation and virtual machine execution.