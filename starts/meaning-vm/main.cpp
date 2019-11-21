#include "concept.hpp"
#include "helpers.hpp"

using namespace std;

#include <iostream>

int main()
{
	r(fruit);
	r(banana);
	r(is);
	banana->link(is, fruit);
	cout << banana << "-" << is << "-" << fruit << endl;
}
