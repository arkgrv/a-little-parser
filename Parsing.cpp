//Adamska-Shalansaska IV/V/MMXXI
#include "functions.cpp" //it isn't a good way, this is only a try

/*Declared Namespace*/
using namespace Stack; //stack functions
using namespace std; //I/O functions
using namespace Token; //token functions
using namespace Vector; //vector functions

int main(){
 try {
		reg v; //the tokens vector
		word s = ""; //the input expr
		int r = 0; //the result

		//cout << str2int(s) << endl;
		//cout << int2str(n) << endl;
		getline(cin, s);
		if (!read(s, v)) return -1;

		print(v); //use this for check the vector content
		if (!calculate(v, r)) return -1;
		cout << "Fine --->" << r << endl;
 }catch (int& val) { cerr << exceptions(val) << endl; }
  return 0;
}

