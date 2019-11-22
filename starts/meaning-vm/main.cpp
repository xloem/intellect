#include "concept.hpp"
#include "helpers.hpp"

using namespace std;

#include <iostream>

int main()
{
	decl(fruit, banana, is);

	banana->link(is, fruit);
	cout << banana << "-" << is << "-" << fruit << endl;
}
