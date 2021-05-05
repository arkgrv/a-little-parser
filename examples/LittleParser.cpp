#include <Parser.h>

using namespace Parser;

int main()
{
	std::string s;

	std::getline(std::cin, s);

	auto tokens = read(s);

	std::cout << Parser::calculate(tokens) << std::endl;
	return 0;
}

