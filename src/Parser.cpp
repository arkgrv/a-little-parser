#include <Parser.h>
#include <cassert>
#include <array>
#include <cmath>

namespace Parser {

	// Operators
	std::ostream& operator<<(std::ostream& ostr, const Token& t)
	{
		ostr << t.value;
		return ostr;
	}

	std::ostream& operator<<(std::ostream& ostr, const std::vector<Token> v)
	{
		for (const auto& x : v) ostr << x << " ";
		ostr << std::endl;
		return ostr;
	}

	std::ostream& operator<<(std::ostream& ostr, const Stack& s)
	{
		ostr << s;
		return ostr;
	}

	double Token::to_value(const std::string& str)
	{
		return std::stod(str);
	}

	std::string Token::to_string(double val)
	{
		return std::to_string(val);
	}

	std::optional<Token> Token::next_token(std::string& str)
	{
		if (str.empty()) return std::nullopt;

		Token t;

		switch (str.front()) {
			case '+':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OP_PLUS, "+");
			case '-':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OP_MINUS, "-");
			case '*':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OP_STAR, "*");
			case '/':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OP_DIV, "/");
			case '^':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OP_EXP, "^");
			case '(':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::OPEN_PAREN, "(");
			case ')':
				str.erase(str.begin(), str.begin() + 1);
				return Token(TokenType::CLOSE_PAREN, ")");
			default: {
				if (isspace(str.front())) {
					str.erase(str.begin(), str.begin() + 1);
					return Token(TokenType::IGN_SPACE);
				}

				// number case
				if (isdigit(str.front())) {
					std::string val;
					bool dec = false;
					while ((isdigit(str.front()) || str.front() == '.')
							&& str.begin() != str.end()) {
						if (str.front() == '.' && dec) continue;
						val.push_back(str.front());
						str.erase(str.begin(), str.begin() + 1);
					}
					return Token(TokenType::NUMBER, val);
				}
				else return std::nullopt;
			}
		}
	}

	struct StackNode {
		Token data;
		StackNode* next;

		StackNode(const Token& data, const StackNode* next)
		: data(data), next(const_cast<StackNode*>(next)) { }

		StackNode() : StackNode(Token(), nullptr) { }
	};

	Stack::Stack() : start(nullptr) { }

	Stack::Stack(Stack&& other) noexcept : start(other.start)
	{
		other.start = nullptr;
	}

	Stack::~Stack()
	{
		auto tmp = start;
		while (nullptr != tmp) {
			tmp = start->next;
			delete start;
			start = tmp;
		}
	}

	void Stack::push(const Token& t)
	{
		start = new StackNode(t, start);
	}

	Token Stack::pop()
	{
		assert(!empty());
		auto ret = start->data;
		auto del = start;

		start = start->next;
		delete del;

		return ret;
	}

	bool Stack::empty() const
	{
		return (nullptr == start);
	}

	std::ostream& Stack::operator<<(std::ostream& ostr)
	{
		auto tmp = start;
		while (nullptr != tmp) {
			ostr << tmp->data << " ";
			tmp = tmp->next;
		}
		ostr << std::endl;
		return ostr;
	}

	std::vector<Token> read(const std::string& text)
	{
		std::vector<Token> res;
		auto str = text;
		for (auto tok = Token::next_token(str); tok.has_value();
				tok = Token::next_token(str)) {
			if (!tok.has_value()) {
				std::cerr << "Unknown token in " << str << std::endl;
				exit(EXIT_FAILURE);
			}
			if (tok->type != Token::TokenType::IGN_SPACE)
				res.push_back(*tok);
		}
		return res;
	}

	double calculate(std::vector<Token> tokens)
	{
		std::array<Token, 5> last_five;
		Stack s;

		double acc;

		while (!tokens.empty()) {
			auto tok = tokens.front();
			s.push(tok);
			tokens.erase(tokens.begin());

			if (tok.type != Token::TokenType::CLOSE_PAREN) continue;

			for (auto it = last_five.rbegin(); it != last_five.rend(); ++it)
				*it = s.pop();

			// syntax check
			if (last_five[0].type != Token::TokenType::OPEN_PAREN) break;
			if (last_five[1].type != Token::TokenType::NUMBER) break;
			if (last_five[2].type != Token::TokenType::OP_PLUS &&
				last_five[2].type != Token::TokenType::OP_MINUS &&
				last_five[2].type != Token::TokenType::OP_STAR &&
				last_five[2].type != Token::TokenType::OP_DIV &&
				last_five[2].type != Token::TokenType::OP_EXP) break;
			if (last_five[4].type != Token::TokenType::CLOSE_PAREN) break;

			// converting value
			auto a = Token::to_value(last_five[1].value);
			auto b = Token::to_value(last_five[3].value);

			// operators check
			switch (last_five[2].type) {
			case Token::TokenType::OP_PLUS: acc = a + b; break;
			case Token::TokenType::OP_MINUS: acc = a - b; break;
			case Token::TokenType::OP_STAR: acc = a * b; break;
			case Token::TokenType::OP_DIV: acc = a / b; break;
			case Token::TokenType::OP_EXP: acc = std::pow(a, b); break;
			default: std::cerr << "Syntax error" << std::endl; exit(EXIT_FAILURE);
			}

			tok.value = Token::to_string(acc);
			tok.type = Token::TokenType::NUMBER;
			s.push(tok);
		}

		if (!tokens.empty()) {
			std::cerr << "Unknown runtime error" << std::endl;
			exit(EXIT_FAILURE);
		}

		auto tok = s.pop();
		if (!s.empty() || tok.type != Token::TokenType::NUMBER) {
			std::cerr << "Unknown runtime error" << std::endl;
			exit(EXIT_FAILURE);
		}
		return Token::to_value(tok.value);
	}
}

