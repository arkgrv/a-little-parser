#include <Parser.h>
#include <cassert>
#include <array>

namespace Parser {

	// Operators
	std::ostream& operator<<(std::ostream& ostr, const Token& t)
	{
		switch (t.type) {
		case Token::TokenType::OP_PLUS: ostr << '+'; return ostr;
		case Token::TokenType::OP_MINUS: ostr << '-'; return ostr;
		case Token::TokenType::OP_STAR: ostr << '*'; return ostr;
		case Token::TokenType::OPEN_PAREN: ostr << '('; return ostr;
		case Token::TokenType::CLOSE_PAREN: ostr << ')'; return ostr;
		case Token::TokenType::NUMBER: ostr << t.value; return ostr;
		default: ostr << ' '; return ostr;
		}
	}

	std::ostream& operator<<(std::ostream& ostr, const std::vector<Token> v)
	{
		for (const auto& x : v) ostr << x << " ";
		ostr << std::endl;
		return ostr;
	}

	int Token::to_int(const std::string& str)
	{
		return std::stoi(str);
	}

	std::string Token::to_string(int val)
	{
		return std::to_string(val);
	}

	std::optional<Token> Token::next_token(std::string& str)
	{
		if (str.empty()) return std::nullopt;

		auto it = str.begin();
		for (; it != str.end(); ++it)
			if (!isspace(*it)) break;
		str.erase(str.begin(), it);

		size_t ns = str.find(' ');
		if (ns == std::string::npos) ns = str.size();

		Token t(str.substr(0, ns));
		str.erase(str.begin(), str.begin() + ns);

		if (t.value.length() == 1) {
			if (isdigit(t.value.front())) {
				t.type = Token::TokenType::NUMBER;
				return t;
			}

			switch (t.value.front()) {
			case '+': t.type = Token::TokenType::OP_PLUS; return t;
			case '-': t.type = Token::TokenType::OP_MINUS; return t;
			case '*': t.type = Token::TokenType::OP_STAR; return t;
			case '(': t.type = Token::TokenType::OPEN_PAREN; return t;
			case ')': t.type = Token::TokenType::CLOSE_PAREN; return t;
			default: return std::nullopt;
			}
		}

		// multi digit number
		for (const auto& x : t.value) {
			if (!isdigit(x)) return std::nullopt;
		}
		t.type = Token::TokenType::NUMBER;
		return t;
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

	void Stack::print() const
	{
		auto ptr = start;
		while (nullptr != start) {
			std::cout << ptr->data << " ";
		}
		std::cout << std::endl;
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
			res.push_back(*tok);
		}
		return res;
	}

	int calculate(std::vector<Token> tokens)
	{
		std::array<Token, 5> last_five;
		Stack s;

		int acc;

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
				last_five[3].type != Token::TokenType::OP_STAR) break;
			if (last_five[4].type != Token::TokenType::CLOSE_PAREN) break;

			// converting value
			auto a = Token::to_int(last_five[1].value);
			auto b = Token::to_int(last_five[3].value);

			// operators check
			switch (last_five[2].type) {
			case Token::TokenType::OP_PLUS: acc = a + b; break;
			case Token::TokenType::OP_MINUS: acc = a - b; break;
			case Token::TokenType::OP_STAR: acc = a * b; break;
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
		return Token::to_int(tok.value);
	}
}

