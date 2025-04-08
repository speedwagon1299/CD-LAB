**Compiler Design Exam Question**

**Instructions:**
Implement the following components of a compiler frontend for a simple pseudo-language that supports record declarations. Your implementation must include:

1. `getNextToken()` function: Performs lexical analysis and returns the next token. Each token must include:

    - Row number
    - Column number
    - Token type (e.g., `KEYWORD`, `IDENTIFIER`, `DATATYPE`, `SYMBOL`, `LITERAL`, etc.)
    - Token value

2. Symbol Table Generator (`symbolTableGen`): Constructs a symbol table from the source code. For each identifier, store:

    - Identifier name
    - Type (e.g., `int`, `char`, `bool`)
    - Row number
    - Column number

3. Recursive Descent Parser (`RDParser`): Parses the source code using the given LL(1) grammar. Include minimal error handling for invalid syntax (e.g., missing `;`, unexpected token, unmatched brackets).

---

**Source Code Example:**

```
record my_rec {
  int a, b[100];
  static char c;
  bool f = false;
}
```

Only record declarations of this kind will be provided.

---

**Grammar:**

```
Program           -> RecordDecl
RecordDecl        -> 'record' ID '{' VarDeclList '}'
VarDeclList       -> VarDecl VarDeclList | ε
VarDecl           -> StaticOpt Type VarDefList ';'
StaticOpt         -> 'static' | ε
Type              -> 'int' | 'char' | 'bool'
VarDefList        -> VarDef VarDefListTail
VarDefListTail    -> ',' VarDef VarDefListTail | ε
VarDef            -> ID ArrayOpt InitOpt
ArrayOpt          -> '[' NUM ']' | ε
InitOpt           -> '=' Val | ε
Val           -> 'true' | 'false' | NUM | LIT
```

---

**Output Requirements:**

-   Lexical Analyzer output with token list
-   Symbol table
-   Parser result (e.g., success or syntax error with line/column info)

---

**Evaluation Criteria:**

-   Correct implementation of the tokenizer and handling of comments, whitespace, and symbols
-   Proper symbol table construction with accurate metadata
-   Functional Recursive Descent Parser with correct grammar application
-   Error detection and messaging for invalid constructs

---

**Note:** Assume the input is error-free unless stated otherwise. Handle basic syntax errors gracefully in the parser.
