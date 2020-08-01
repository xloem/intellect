#include <library/string.hpp>

#include "test.hpp"

using namespace library;

int main()
{
	string default_constructed;
	worry(default_constructed.size(), "string not constructed empty");

	string from_cstring("hello, world");
	worry(from_cstring.size() != 12, "cstring has incorrect size");
	worry(from_cstring != "hello, world", "cstring compares false to content");

	string from_literal = "hello, world"_s;
	worry(from_literal.size() != 12, "literal string has incorrect size");
	worry(from_literal != "hello, world", "literal string compares false to content");

	string from_mutable_cstring((char*)"hello, world");
	worry(from_cstring.size() != 12, "mutable cstring has incorrect size");
	worry(from_cstring != "hello, world", "mutable cstring comparse false to content");

	string from_true((bool)true);
	string from_false((bool)false);
	worry(from_true != "true", "bool string not 'true'");
	worry(from_false != "false", "bool string not 'false'");

	string from_integer((unsigned char)0xc7);
	worry(from_integer != "c7", "byte string '" + from_integer + "' instead of 'c7'");
	worry(from_integer.to_unsigned_char(16) != 0xc7, "byte from string " + string(from_integer.to_unsigned_char(16), 16, true) + " instead of 0xc7");

	from_integer = string((unsigned long)3141592653);
	worry(from_integer != "3141592653", "integer string '" + from_integer + "' instead of '3141592653'");
	worry(from_integer.to_unsigned_long() != 3141592653, "integer from string " + string(from_integer.to_unsigned_long()) + " instead of 3141592653");

	from_integer = string((unsigned long long)5141592653589793238);
	worry(from_integer != "5141592653589793238", "integer string '" + from_integer + "' instead of '5141592653589793238'");
	worry(from_integer.to_unsigned_long_long() != 5141592653589793238, "integer from string " + string(from_integer.to_unsigned_long_long()) + " instead of 5141592653589793238");

	from_integer = string((long long)-3141592653589793238, 10, true);
	worry(from_integer != "-0d3141592653589793238", "integer string '" + from_integer + "' instead of '-0d3141592653589793238'");
	worry(from_integer.to_signed_long_long() != -3141592653589793238, "integer from string " + string(from_integer.to_signed_long_long()) + " instead of -3141592653589793238");

	string from_real(3.141592);
	worry(from_real != "3.141592", "real string '" + from_real + "' instead of '3.141592'");
	worry(from_real.to_double() != 3.141592, "real from string " + string(from_real.to_long_double()) + " instead of 3.141592");

	from_real = string(3.14159265358979L, 10, true, 14);
	worry(from_real != "0d3.14159265358979", "real string '" + from_real + "' instead of '0d3.14159265358979'");
	worry(from_real.to_long_double() != 3.14159265358979L, "real from string " + string(from_real.to_long_double()) + " instead of 3.14159265358979");

	string from_pointer((void*)0xaaaa480);
	string plan = sizeof(void*)==4 ? "0x0aaaa480" : "0x000000000aaaa480";
	worry(from_pointer != plan, "pointer string '" + from_pointer + "' instead of '" + plan + "'");
	worry(from_pointer.to_pointer() != (void*)0xaaaa480, "pointer from string " + string(from_pointer.to_pointer()) + " instead of " + plan);

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

	hello_world = string("howdy there, world");
	hello_world.splice(1, 10, "ello", 4);
	worry(hello_world != "hello, world", "splicing shorter failed");

	hello_world = string("hey, world");
	hello_world.splice(1, 2, "ello", 4);
	worry(hello_world != "hello, world", "splicing longer failed");


	stderr::line("string passed test");
}
