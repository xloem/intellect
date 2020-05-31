#include <iostream>
#include <string>

#include "reference.hpp"

using namespace std;

int main(int argc, char ** argv)
{
	reference concept, is, object, chair, wood, material;
	reference::set(chair, is, object);
	reference::set(concept, is, chair);
	reference::set(concept, material, wood);

	((string&)wood) = "tree-body";

	cout << (string)reference::get(concept, material) << endl;
}
