#include "expressions.hpp"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{
	vector<any> arguments;
	arguments.emplace_back(1);
	arguments.emplace_back(2);
	arguments.emplace_back(3);
	operators::plus_any<int>.solve(arguments, 0);
	assert(any_cast<int>(arguments[0]) == 5);

	cerr << "Success" << endl;
}
