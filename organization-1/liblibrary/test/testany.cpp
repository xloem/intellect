#include <library/any.hpp>
#include <library/type_definition.cpp>

#include "test.hpp"

using namespace library;

int main()
{
	any str = string("hello");
	any number = 3;

	worry(str.type() != type<string>(), string("string type is ") + str.type().name + " instead of " + type<string>().name);
	worry(number.type() != type<decltype(3)>(), string("number type is ") + number.type().name + " instead of " + type<decltype(3)>().name);
	worry(str.reference<string>() != "hello", "string content is '" + str.reference<string>() + "' instead of 'hello'");
	worry(number.reference<decltype(3)>() != 3, string("number content is ") + number.reference<decltype(3)>() + " instead of 3");
	
	any temp = str;

	worry(temp.type() != type<string>(), string("assigned string type is ") + temp.type().name + " instead of " + type<string>().name);
	worry(temp.reference<string>() != "hello", "assigned string content is '" + temp.reference<string>() + "' instead of 'hello'");

	str = number;
	number = temp;

	temp = str;
	str = number;
	number = temp;

	worry(str.type() != type<string>(), string("string type is ") + str.type().name + " instead of " + type<string>().name);
	worry(number.type() != type<decltype(3)>(), string("number type is ") + number.type().name + " instead of " + type<decltype(3)>().name);
	worry(str.reference<string>() != "hello", "string content is '" + str.reference<string>() + "' instead of 'hello'");
	worry(number.reference<decltype(3)>() != 3, string("number content is ") + number.reference<decltype(3)>() + " instead of 3");

	call_tracker other_caller;
	any caller(type<call_tracker>());
	worry(true != called_default, "call_tracker not default constructed");
	worry(false != called_assign, "call_tracker already assigned");
	worry(false != called_copy, "call_tracker copy constructed");
	worry(false != called_destroy, "call_tracker already destroyed");
	caller = other_caller;
	worry(true != called_assign, "call_tracker assignment never called");
	worry(false != called_copy, "call_tracker copy constructed");
	worry(false != called_destroy, "call_tracker already destroyed");
	caller = 3;
	worry(true != called_destroy, "call_tracker never destroyed");
	worry(false != called_copy, "call_tracker copy constructed");
	caller = call_tracker{};
	worry(true != called_copy, "call_tracker never copy constructed");

	stderr::line("any passed test");
}
