### **Instructions**

Implement the following compiler frontend components for a simple pseudo-language with basic types, assignments, and conditional statements.

Your implementation must include:

1. `getNextToken()` – Returns:

    - Token type (e.g., `KEYWORD`, `IDENTIFIER`, `DATATYPE`, `SYMBOL`, `LITERAL`)
    - Row number
    - Column number
    - Token value

2. `symbolTableGen()` – Builds a symbol table containing:

    - Identifier name
    - Type (int, char, bool)
    - Row, Column

3. `RDParser()` – A Recursive Descent Parser based on the LL(1) grammar below. Handle basic syntax errors (e.g., missing semicolon, unmatched braces, etc.)

---

### **Example Source Code**

```c
function main() {
  int x;
  bool flag = true;
  x = 10;

  if (flag) {
    x = 20;
  } else {
    x = 30;
  }
}
```

---

### **Grammar**

```
Program            -> FunctionDecl
FunctionDecl       -> 'function' ID '(' ')' '{' StmtList '}'
StmtList           -> Stmt StmtList | ε
Stmt               -> Declaration | Assignment | IfStmt
Declaration        -> Type ID InitOpt ';'
Assignment         -> ID '=' Expr ';'
IfStmt             -> 'if' '(' Expr ')' '{' StmtList '}' ElsePart
ElsePart           -> 'else' '{' StmtList '}' | ε
Type               -> 'int' | 'char' | 'bool'
InitOpt            -> '=' Expr | ε
Expr               -> NUM | LIT | 'true' | 'false' | ID
```

> **Note:** This grammar does not support arithmetic, just basic assignment and boolean expressions.

---

### **Token Types**

You must recognize the following as tokens:

-   **Keywords:** `function`, `int`, `char`, `bool`, `true`, `false`, `if`, `else`
-   **Symbols:** `{`, `}`, `(`, `)`, `=`, `;`
-   **Identifiers:** (e.g., `main`, `x`, `flag`)
-   **Literals:** e.g., `'A'`, `42`

---

### **Required Output**

1. **Token list**: One token per line with row, column, type, and value
2. **Symbol table**: Type + identifier + row/column
3. **Parser result**: Success or error with row/column info

---
