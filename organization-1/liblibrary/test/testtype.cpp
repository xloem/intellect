#include <library/type_definition.cpp>

#include "test.hpp"

using namespace library;

int main()
{
	worry(type<string>().size != sizeof(string), "size of string type incorrect");

	worry(type<int(*)()>() != type(&main), "identical type calls differ");
	worry(type(&main) == type<string>(), "differing type calls identical");

	worry(type<void>().size != 0, "void type has size");

	unsigned char storage[sizeof(call_tracker)];
	worry(type<call_tracker>().size != sizeof(storage), "size of call tracker incorrect");

	worry(called_default, "called_default already set");
	type<call_tracker>().construct_default(storage);
	worry(!called_default, "failed to call default constructor");

	worry(called_destroy, "called_destroy already set");
	type<call_tracker>().destroy(storage);
	worry(!called_destroy, "failed to call destructor");

	worry(called_copy, "called_copy already set");
	type<call_tracker>().construct_copy(storage, storage);
	worry(!called_copy, "failed to call copy constructor");

	worry(called_assign, "called_assign already set");
	type<call_tracker>().assign(storage, storage);
	worry(!called_assign, "failed to call assignment operator");

	type<call_tracker>().destroy(storage);


	stderr::line("type passed test");
}
