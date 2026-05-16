# CVM-

# Input and Output Format

## Input Format

The lexer accepts source code as a single string.

Example input:

```txt
var x = 10;

agar (x >= 5) {
    x = x + 1;
}
```

The source code is passed into the lexer constructor:

```cpp
Lexer lexer(source);
vector<Token> tokens = lexer.scan();
```

---

## Output Format

The lexer outputs a:

```cpp
vector<Token>
```

Each token contains:

```cpp
class Token {
public:
    TokenType type;
    string lexeme;
    int line;
};
```

---

## Output Example

### Input

```txt
var x = 10;
```

### Output

```txt
type: VAR, value: var, line: 1
type: IDENTIFIER, value: x, line: 1
type: EQUAL, value: =, line: 1
type: NUMBER, value: 10, line: 1
type: SEMICOLON, value: ;, line: 1
type: TOKEN_EOF, value: , line: 1
```

---

## String Example

### Input

```txt
var name = "ansh";
```

### Output

```txt
type: VAR, value: var, line: 1
type: IDENTIFIER, value: name, line: 1
type: EQUAL, value: =, line: 1
type: STRING, value: ansh, line: 1
```

---

## Conditional Example

### Input

```txt
agar (x >= 10) {
    x = x + 1;
}
```

### Output

```txt
type: AGAR, value: agar, line: 1
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
