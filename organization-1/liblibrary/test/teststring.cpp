#include <library/string.hpp>

using namespace library;

void error(bool worry, string what)
{
	if (!worry) { return; }
	stderr::line(what);
	throw false;
}

int main()
{
	string default_constructed;
	error(default_constructed.size(), "string not constructed empty");

	string from_cstring("hello, world");
	error(from_cstring.size() != 12, "cstring has incorrect size");
	error(from_cstring != "hello, world", "cstring comparse false to content");

	string from_mutable_cstring((char*)"hello, world");
	error(from_cstring.size() != 12, "mutable cstring has incorrect size");
	error(from_cstring != "hello, world", "mutable cstring comparse false to content");

	string from_true((bool)true);
	string from_false((bool)false);
	error(from_true != "true", "bool string not 'true'");
	error(from_false != "false", "bool string not 'false'");

	string from_integer(3141593);
	error(from_integer != "3141593", "integer string not '3141593'");

	string from_real(3.141593);
	error(from_real != "3.141593", "real string '" + from_real + "' instead of 3.141593");

	string from_pointer((void*)0xaaaa480);
	error(from_pointer != "0xaaaa480", "pointer string not '0xaaaa480'");

	string hello_world = string("hello, ") + string("world");
	error(hello_world != "hello, world", "operator+ not 'hello, world'");

	hello_world = "hello, ";
	error(hello_world != "hello, ", "string assignment failed");

	hello_world += "world";
	error(hello_world != "hello, world", "operator+= not 'hello, world'");

	for (unsigned long i = 0; i < 12; ++ i) {
		error(hello_world[i] != "hello, world"[i], "operator[] incorrect");
	}

	auto ptr = "hello, world";
	auto data = hello_world.data();
	for (auto character : hello_world) {
		error(character != *ptr, "string iteration incorrect");
		error(*data != *ptr, "string data incorrect");
		++ ptr;
		++ data;
	}

	stderr::line("string passed test");
}
