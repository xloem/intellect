#include "expressions.hpp"
#include <cassert>
//#include <library/assert.hpp>
#include <library/string.hpp>
//#include <library/heapvector.hpp>
//#include <vector>
//using namespace std;
using namespace library;

#include <typeinfo>

int main()
{
	// slow to compile on arm64
	
	stdout::line("Hello, world!");

	/*
	// to do this all we need is the templates with operators
	// not the implementations of their fucntions that use the enums
	expression_templated<literal, int const> int1 = 1;
	expression_templated<literal, int const> int2 = 2;
	auto sum = int1 + int2;
	*/
	//stdout::line(sum);
	//stdout::line(typeid(sum).name());

	/*
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
	*/
}
