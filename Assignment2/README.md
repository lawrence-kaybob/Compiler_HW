# Assignment #2 : Mathematical Expression Evaluator

### Purpose

Using recurvice decent parsing method, we try to develop interpreter for simple expressions.

### Requirements

Expressions includes constants, variables, binary operators, unary operators, and assignment expression

- Operator
  - Binary operator : + - * / = (assignment)
  - Unary operator : -
    - e.g.) -5 + -10 ==> -15
- Constant
  - Intenger
  - Real number
- Assignment operator has **right-associativity**
- Lexical pattern of variables, constants are same as [Assignment #1](https://github.com/lawrence-kaybob/Compiler_HW/tree/master/Assignment1)

### Error Handling

- If variables are used without initialization, it should return an error as *"Not Initialized"*
- If syntax is wrong, it should return an error as *"Syntax Error"*
- If word used is wrong (which means if wrong token is used), it should return an error as *"Lexical Error"*

### Idea

To be explained.

### Limitations

To be added

### Example

```
> 10
< 10
> 10+5
< 15
> value = 10
< 10
> value +3
< 13
> i = j = 20
< 20
> (value + value) *3
< 60
> value = value + i
< 30
> -value - 100
< -130
```

