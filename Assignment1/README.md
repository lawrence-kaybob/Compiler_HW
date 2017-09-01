# Assignment #1 : Mini C Lexical Analyzer

### Purpose

Using Lex and small C code provided, create a program that analyzes words of C code

### Terminology

- Keywords : Reserved words
  - ex) *int, char, double, void, if, else, for, while, return*
- Indentifiers (for variables or function name)
  - Only can use English capital, lowercase, numbers, and underscore( _ )
  - First letter should be caps, lowercase, and underscore (numbers are not allowed)
  - It cannot be composed with underscore
  - There is no limitation on length, but only save 16 characters of it
- Integers
  - Sign (+, -) is allowed
  - No leading zeros
  - There is no limitation on length of numbers, but only store up to 10 digits (truncate after 11th digit)
- Float-point numbers
  - Follows float-point number rule of C program
- Strings
  - All letters could be included inside double quotation mark, except double quotation mark itself should be escaped (\\")
  - You don't have to consider multiple line string
- Operators
  - +, -, *, /
  - ++, --
  - <, <=, >, >=, ==, !=
  - =, +=, -=, *=, /=, %=
  - !, &&, ||
- Other special characters
  - Other than operators
  - ex) " \ [ ] ; ,
- Comments
  - /* */
- Notice
  - Unless addressed, program follows the basic C language grammar.

### Idea

To be added

### Limitations

To be added

### Example

- On *Flex-Bison* project "Settings", go to "Debugging", and set "Command Argument" with C code to test
- The sample result would be like below

```Bash
TOKEN           LEXEME
-----           ------
<COMMENT>               /**
* Test Data 1
* Checking basic information
*/
<INCLUDE>               #include
<STANDARD HEADER>               <stdio.h>
<INT>           int
<IDENTIFIER, 1>         main
<LEFTPARANTEHSIS>               (
<VOID>          void
<RIGHTPARANTEHSIS>              )
<LEFTBRACKET>           {
<INT>           int
<IDENTIFIER, 2>         age
<COMMA>         ,
<IDENTIFIER, 3>         i
<SEMICOLON>             ;
<CHAR>          char
<IDENTIFIER, 4>         initial
<LEFTBRACE>             [
<DIGIT>         3
<RIGHTBRACE>            ]
<ASSIGN>                =
<LEFTBRACKET>           {
<DIGIT>         0
<RIGHTBRACKET>          }
<SEMICOLON>             ;
<IDENTIFIER, 5>         printf
<LEFTPARANTEHSIS>               (
<STRING, 1>             "Enter your initial (first 3 letters) : "
<RIGHTPARANTEHSIS>              )
<SEMICOLON>             ;
...
```

