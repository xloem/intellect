#include <library/string.hpp>

#include "test.hpp"

using namespace library;

int main()
{
	string default_constructed;
	worry(default_constructed.size(), "string not constructed empty");

	string from_cstring("hello, world");
	worry(from_cstring.size() != 12, "cstring has incorrect size");
	worry(from_cstring != "hello, world", "cstring comparse false to content");

	string from_mutable_cstring((char*)"hello, world");
	worry(from_cstring.size() != 12, "mutable cstring has incorrect size");
	worry(from_cstring != "hello, world", "mutable cstring comparse false to content");

	string from_true((bool)true);
	string from_false((bool)false);
	worry(from_true != "true", "bool string not 'true'");
	worry(from_false != "false", "bool string not 'false'");

	string from_integer(3141593);
	worry(from_integer != "3141593", "integer string not '3141593'");

	//string from_real(3.141593);
	//worry(from_real != "3.141593", "real string '" + from_real + "' instead of 3.141593");

	string from_pointer((void*)0xaaaa480);
	string plan = sizeof(void*)==4 ? "0x0aaaa480" : "0x000000000aaaa480";
	worry(from_pointer != plan, "pointer string '" + from_pointer + "' instead of '" + plan + "'");

	string hello_world = string("hello, ") + string("world");
	worry(hello_world != "hello, world", "operator+ not 'hello, world'");

	hello_world = "hello, ";
	worry(hello_world != "hello, ", "string assignment failed");

	hello_world += "world";
	worry(hello_world != "hello, world", "operator+= not 'hello, world'");

	hello_world = string({"hello", "world"}, string(", "));
	worry(hello_world != "hello, world", "joining failed");

	for (unsigned long i = 0; i < 12; ++ i) {
		worry(hello_world[i] != "hello, world"[i], "operator[] incorrect");
	}

	auto ptr = "hello, world";
	auto data = hello_world.data();
	for (auto character : hello_world) {
		worry(character != *ptr, "string iteration incorrect");
		worry(*data != *ptr, "string data incorrect");
		++ ptr;
		++ data;
	}

	stderr::line("string passed test");
}
