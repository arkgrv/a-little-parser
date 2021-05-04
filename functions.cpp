//Adamska-Shalansaska IV/V/MMXXI
//header-functions
#include "header.h"

/*Handling enum outputs, if it return the element position in standard output, i define a String return*/
std::ostream& operator << (std::ostream& out, const Token::kind value) {
	static std::map<Token::kind, word> strings;
	if (strings.size() == 0) {
#define Insert_Element(p) strings[p] = #p
		Insert_Element(Token::open_P);
		Insert_Element(Token::close_P);
		Insert_Element(Token::number);
		Insert_Element(Token::sum);
		Insert_Element(Token::difference);
		Insert_Element(Token::multiplex);
		Insert_Element(Token::foreign);
		Insert_Element(Token::empty);
#undef Insert_Element
	}
	return out << strings[value];
}

/*Handling Vector*/

//insert an element inside the vector
void Vector::push_back(Vector::reg& v, const Vector::elem e) { v.push_back(e); }

//print the vector, if it's Empty an exception will throw
void Vector::print(const Vector::reg& v) {
	if (v.empty()) throw - 2;
	for (unsigned int i = 0; i < v.size(); i++) std::cout << v.at(i).val << " " << v.at(i).k << (i < (v.size() - 1) ? " " : "\n");
}

/*Handling Stack*/

//insert an element inside the stack
void Stack::push(Stack::stack& s, const Stack::elem t) {
	Stack::cell* aux = new cell;
	aux->e = t;
	aux->next = s;
	s = aux;
}

//check if it's empty
bool Stack::isEmpty(const Stack::stack& s) { return(s == nullptr); }

//pull of the top element, if it's empty an exception will throw
Stack::elem Stack::pop(Stack::stack& s) {
	if (isEmpty(s)) throw - 1;
	Stack::stack aux = s;
	Stack::elem e_aux = aux->e;
	s = s->next;
	delete aux;
	return e_aux;
}

//print the stack, if it's empty an exception will throw
void Stack::print(const Stack::stack& s) {
	if (isEmpty(s)) throw - 1;

	Stack::stack aux = s;
	while (aux != nullptr) {
		std::cout << aux->e.val << " " << aux->e.k << (aux->next != nullptr ? " " : "\n");
		aux = aux->next;
	}
}

/*Handling Tokens*/

//convert a value from string to integer
int Token::str2int(const word& s){
	int n = 0;
	std::istringstream itmp(s);
	itmp >> n;
	return n;
}

//convert a value from integer to string
word Token::int2str(const int& r) {
	std::ostringstream otmp;
	otmp << r;
	return(otmp.str());
}

//find the next token in expression str
bool Token::nextToken(word& str, token& t) {
	int proxspazio = 0, i;

	for (i = 0; i < str.size(); i++) if (str[i] != ' ') break;
	str.erase(0, i);
	if (str.empty()) return false;
	proxspazio = str.find(" ", i);
	if (proxspazio == word::npos) proxspazio = str.size();
	t.val = str.substr(0, proxspazio);
	str.erase(0, proxspazio);
	if (t.val.size() == 1) {
		if (t.val[0] >= '0' && t.val[0] <= '9') t.k = Token::number; //it's marked as a numer if condition is true
		else{ //therefore, may it's an arithmetic operator or brackets
			switch (t.val[0]) {
				case'+': t.k = Token::sum; break;
				case'-': t.k = Token::difference; break;
				case'*': t.k = Token::multiplex; break;
				case'(': t.k = Token::open_P; break;
				case')': t.k = Token::close_P; break;
				default: t.k = Token::foreign; break;
			}
		}
		return(t.k != Token::foreign);
	}
	for (i = 0; i < t.val.size(); i++) if (t.val[i] < '0' || t.val[i] > '9') break;
	if (i >= t.val.size()) {
		t.k = Token::number;
		return true;
	}
	t.k = Token::foreign;
	return false;
}

/*Tokenizer and Parsing*/

//lexical analysis
bool read(const word& str, Vector::reg& v){
	Token::token tok;
	word localString = str;

	while (Token::nextToken(localString, tok)) Vector::push_back(v, tok);
	return((localString.size() == 0) && (tok.k != Token::foreign));
}

//syntactic analysis and calculus
bool calculate(Vector::reg v, int& r) {
	Token::token lastFive[5], tok;
	Stack::stack s = nullptr;
	int a = 0, b = 0, c = 0, i = 0, j = 0;  //a && b numbers, c is the result.... i && j are index.. i is used for lastFive[5] j for iterate vector

	while (!v.empty()) {
		tok = v.at(j);
		Stack::push(s, tok);
		v.erase(v.begin() + 0); //is da wea?
		if(tok.k != Token::close_P) continue;

		for (i = 4; i >= 0; i--) {
			lastFive[i].k = Token::foreign;
			lastFive[i] = Stack::pop(s);
		}
		/*std::cout << lastFive[0].val << " " << lastFive[1].val << " " << lastFive[2].val << " " << lastFive[3].val << " " << lastFive[4].val << std::endl;*/ //leave the comments if u're not sure of functioning

		//check of syntax--->parsing
		if (lastFive[0].k != Token::open_P) break;
		if (lastFive[1].k != Token::number) break;
		if ((lastFive[2].k != Token::sum) && (lastFive[2].k != Token::difference) && (lastFive[2].k != Token::multiplex)) break;
		if (lastFive[3].k != Token::number) break;
		if (lastFive[4].k != Token::close_P) break;

		//convert the value from string to integer
		a = Token::str2int(lastFive[1].val);
		b = Token::str2int(lastFive[3].val);
		
		//check the operator (+, -, *)
		switch (lastFive[2].k) {
			case Token::sum: c = a + b; break;
			case Token::difference: c = a - b; break;
			case Token::multiplex: c = a * b; break;
			default: exit(1);
		}
		tok.val = Token::int2str(c);
		tok.k = Token::number;
		Stack::push(s, tok);
		/*Stack::print(s);*/ //delete comments if u wanna debug the stack
	}
	if (!v.empty()) return false;
	tok = Stack::pop(s);
	if ((!Stack::isEmpty(s)) || (tok.k != Token::number)) return false;
	r = Token::str2int(tok.val);
	return true;
}

//check the value for throw the right exception
word exceptions(int& val) {
	switch (val) {
		case -1: return STACK;
		case -2: return VECTOR;
		default: return "UNKNOW ERROR";
	}
}