#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <optional>

namespace Parser {
	struct Token {
		// Token type specification
		enum class TokenType {
			OPEN_PAREN,
			CLOSE_PAREN,
			NUMBER,
			OP_PLUS,
			OP_MINUS,
			OP_STAR,
			OP_DIV,
			OP_EXP,
			IGN_SPACE,
			TBD,
			EMPTY
		};
		
		Token(const TokenType& t, const std::string& value)
		: type(t), value(value) {}

		Token(const TokenType& t) : Token(t, "") { }

		Token() : Token(TokenType::EMPTY, "") { }

		explicit Token(const std::string& value)
		: Token(TokenType::TBD, value) {}

		static double to_value(const std::string& str);

		static std::string to_string(double val);

		static std::optional<Token> next_token(std::string& str);

		TokenType type;
		std::string value;
	};

	class Stack {
	public:
		Stack();

		Stack(const Stack& other) = delete;

		Stack(Stack&& other) noexcept;

		~Stack();

		void push(const Token& t);

		Token pop();

		bool empty() const;

		std::ostream& operator<<(std::ostream& ostr);

	private:
		struct StackNode* start;
	};

	//Auxiliary functions
	std::vector<Token> read(const std::string& text);
	double calculate(std::vector<Token> tokens);

	std::ostream& operator<<(std::ostream& ostr, const Token& t);
	std::ostream& operator<<(std::ostream& ostr, const std::vector<Token> v);
	std::ostream& operator<<(std::ostream& ostr, const Stack& s);
}
#endif
