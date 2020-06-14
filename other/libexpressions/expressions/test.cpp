#include "expressions.hpp"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{
	vector<any> arguments, verify;
	arguments.emplace_back(1);
	arguments.emplace_back(2);
	arguments.emplace_back(3);
	// please use a arrays of any's rather than pasting
	// every operator and type in, to test all types

	assert(operators::plus_any<int>.solve(arguments, 0)[0] == 5);
	verify = arguments;
	assert(operators::plus_any<int>.solve(arguments, 0) == verify);
	assert(operators::plus_any<int>.solve(arguments, 1) == verify);
	assert(operators::plus_any<int>.solve(arguments, 2) == verify);

	arguments[0] = true;
	assert(operators::equal_to_any<int>.solve(arguments, 0)[0] == false);
	arguments[0] = true;
	assert(operators::equal_to_any<int>.solve(arguments, 1)[1] == verify[2]);
	arguments[1] = verify[1];
	assert(operators::equal_to_any<int>.solve(arguments, 2)[2] == verify[1]);

	cerr << "Success" << endl;
}
