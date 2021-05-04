# a-little-parser

## 1. Introduction
This **simple C++ parser project** is based on ``@Adamska570``'s project. It implements a very simple arithmetic expressions parser and evaluator completely written in C++. As of today, the parser supports only **basic operations** such as: <br>
- addition;
- subtraction;
- multiplication;
- division;
- exponentiation
<br>
Any other operation is currently unsupported.

<br>
BEWARE: with long inputs, the parser may fail! It has **not been fuzzed or thoroughly tested**.

## 2. Purpose
The purpose of this rewrite is simply of demonstrative nature. Idiomatic C++ can make the expression of computation simpler than C-Style C++ or mixed-style C++, which often is the **most teached** form of C++.

## 3. Compilation
Simply clone this repo and use CMake to generate a buildable project. Then, run the build accordingly.
