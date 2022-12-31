# a-little-parser

## 1. Introduction
This **simple Rust parser project** is based on ``@Adamska570``'s project. It implements a very simple arithmetic expressions parser and evaluator completely written in Rust. As of today, the parser supports only **basic operations** such as: <br>
- addition;
- subtraction;
- multiplication;
- division;
- power
<br>
Any other operation is currently unsupported.

<br>
BEWARE: with long inputs, the parser may fail! It has **not been fuzzed or thoroughly tested**.

## 2. Purpose
The purpose of this rewrite is simply of demonstrative nature. Idiomatic Rust can make the expression of computation simpler than C-Style C++ or C++, which often is the **most teached** form of C++.

## 3. Compilation
Simply clone this repo and use `cargo build` to build the code in Debug mode. Otherwise, if you plan using this parser for **something useful**, you can build it in release mode with `cargo build --release`.