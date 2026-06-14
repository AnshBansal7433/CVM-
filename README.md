# CVM - Custom Virtual Machine

A small interpreted programming language written in **C++** featuring a complete compilation pipeline:

* Lexer
* Parser
* Abstract Syntax Tree (AST)
* Bytecode Compiler
* Stack-Based Virtual Machine

The goal of this project is to understand how programming languages work internally—from lexical analysis and parsing to bytecode generation and execution.

---

# Features

* Variable declarations
* Variable assignment
* Arithmetic expressions
* Boolean expressions
* Comparison operators
* Logical operators
* If / Else If / Else
* While loops
* For loops
* User input (`scan`)
* Output (`show`)
* Bytecode compilation
* Stack-based virtual machine execution

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
| -------- | ------- |
| aur      | AND     |
| ya       | OR      |

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

| Keyword | Description          |
| ------- | -------------------- |
| var     | Variable declaration |
| show    | Print output         |
| scan    | Read input           |
| when    | If statement         |
| elif    | Else-if statement    |
| other   | Else statement       |
| jabtak  | While loop           |
| for     | For loop             |
| aur     | Logical AND          |
| ya      | Logical OR           |
| true    | Boolean true         |
| false   | Boolean false        |

---

# Example Program

```c
var sum = 0;

for (var i = 1; i <= 5; i = i + 1;) {
    sum = sum + i;
}

show sum;
```

Output:

```text
15
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

# Parser & AST

The parser converts the token stream into an Abstract Syntax Tree (AST).

Example:

```c
var x = (3 + 5) * (6 + 7);
show x;
```

Example AST:

```text
VarStmt(x)
  Binary(*)
    Binary(+)
      Number(3)
      Number(5)
    Binary(+)
      Number(6)
      Number(7)
ShowStmt
  Variable(x)
```


---

# Bytecode Generation

The compiler converts the AST into bytecode instructions executed by the VM.

Example:

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
    Output
```

---

# Project Structure

```text
.
CVM/
│
├── common/
│   └── memory.h
│
├── compiler/
│   └── compiler.h
│
├── examples/
│
├── frontend/
│   ├── ast.h
│   ├── lexer.h
│   ├── lexer.cpp
│   ├── parser.h
│   ├── parser.cpp
│   └── token.h
│
├── vm/
│   ├── chunk.h
│   ├── value.h
│   ├── vm.h
│   └── vm.cpp
│
├── main.cpp
├── build.bat
└── cvm.exe
```

---

# Build

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

or

```bat
cvm.exe program.txt
```

---


# Inspiration

CVM is a learning project built to explore how interpreters, compilers, and virtual machines work under the hood.
