//Adamska-Shalansaska IV/V/MMXXI
#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//standard containers
#include <cstdlib> //standard library of C language
#include <iostream> //standard I/O C++ language
#include <map> //handling enum values
#include <string> //handling string functions
#include <sstream> //handling stringstream
#include <vector> //handling vector

//alias for std::string type
typedef std::string word;

//type of exceptions
const word STACK = "EMPTY_STACK"; //The stack is empty
const word VECTOR = "EMPTY_VECTOR"; //The vector is empty

//handling Token
namespace Token {
	enum kind { open_P, close_P, number, sum, difference, multiplex, foreign, empty }; 

	//Define the token type
	struct token {
		word val;
		kind k; // enum value
	};

	int str2int(const word&); //convert a value from word to an integer

	word int2str(const int&); //convert a value from integer to a word (string)

	bool nextToken(word&, token&); //search into an expression the next token
}

//handling Stack
namespace Stack {
	//create an alias of token for the cell struct
	typedef Token::token elem;

	//Define the cell type, the cells will create the stack
	struct cell {
		elem e;
		cell* next;
	};

	//create an alias for cell* type
	typedef cell* stack;

	void push(stack&, const elem); //insert an element inside
	void print(const stack&); //print the stack

	elem pop(stack&); //extraction of the top element

	bool isEmpty(const stack&); //stack is Empty? or not?
}

//handling vector
namespace Vector{
	typedef Token::token elem; //alias for define the vector
	typedef std::vector<elem> reg;//alias for std::vector<elem>

	void push_back(reg&, const elem); //insert element
	void print(const reg&); //print the vector
}

//Auxiliary functions
bool read(const word&, Vector::reg&); //this is the Tokenizer
bool calculate(Vector::reg, int&); //this is the Parser

word exceptions(int&); //throw an exception
#endif //!FUNCTIONS_H
