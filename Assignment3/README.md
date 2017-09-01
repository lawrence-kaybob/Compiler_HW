# Assignment #3 : Compiler & Interpreter Development
### Purpose

Using LALR(Lookahead Left to Right - rightmost deriviation-), this assignment is to develop interpreter for a simple interpreted language. It consist of two steps. It constructs syntax tree for statement (or statements), evaluate those, and return results.

### Requirements

Constants, variables, and expressions should be handled as [Assignment 1](https://github.com/lawrence-kaybob/Compiler_HW/tree/master/Assignment1) and [Assignment 2](https://github.com/lawrence-kaybob/Compiler_HW/tree/master/Assignment2).

1. Constants

   - Supports interger and real number
   - No need to support exponential form of real number

2. Variables

   - Variables can be used without initalization

   - Variable's type is determined by the stored data (Can be changed during execution)
     e.g.)

     ```
     i = 10;		// Variable i's type is set to int
     i = 3.4;	// Variable i's type is changed to double
     ```

3. Expression and operators

   - Basic arithmetic operators : +, -, *, /
   - Comparison operators : > >= < <= == !=
   - Assignment operator : =
   - If interger and real number is mixed, the result should be in a real number form

4. Statement

   - Every statement should be end with semicolon. It also means it should support multi-line statement
     e.g.)

     ```
     a = a +
     		b;	// It is also same as a = a + b;
     ```

   - Statement list does not grouped with braces *{ }*.  Instead, it is terminated with ***end***

   - ***If*** block

     ```
     if(exp)
     	statement list
     else
     	statement list
     end
     ```

   - ***while*** block

     ```
     while (exp)
     	statement list
     end
     ```

5. Function

   - It starts with ***def*** and finishes with ***end***

   - It does not define type of parameters and return value

   - *Call by value* method is used for parameter passing

   - Local variable is allowed

   - Example

     ```
     def f(a, b)
     	local max
     	if(a > b)
     		max = a;
         else
         	max = b;
         end
         return max;
     end
     ```

6. Error Detection

   - Return *Lexical Error* or *Syntax Error*, if it is detected

7. Error Recovery

   - In case of ***if***, ***while***, and ***def***, retrict the failure to a erroneous statement, so that rest of the statements works as intended

### Idea

To be added

### Limitations

To be added

### Example

```
?-  3;
< 3
3+5;
< 8
a=3;
< 3
b=a+2;
< 5
if(a>b)
> max = a;
> else
> max = b;
> end
if
?- max;
< 5
```



