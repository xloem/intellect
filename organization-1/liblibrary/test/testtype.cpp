#include <library/string.hpp>

#include <library/type_definition.cpp>

using namespace library;

void error(bool worry, string what)
{
	if (!worry) { return; }
	stderr::line(what);
	throw false;
}


bool called_default = false;
bool called_copy = false;
bool called_assign = false;
bool called_destroy = false;
class call_tracker
{
public:
	call_tracker()
	{
		called_default = true;
	}

	call_tracker(call_tracker const &)
	{
		called_copy = true;
	}

	call_tracker& operator=(call_tracker const &)
	{
		called_assign = true;
		return *this;
	}

	~call_tracker()
	{
		called_destroy = true;
	}
};

int main()
{
	error(type<string>().size != sizeof(string), "size of string type incorrect");

	error(type<int(*)()>() != type(&main), "identical type calls differ");
	error(type(&main) == type<string>(), "differing type calls identical");

	error(type<void>().size != 0, "void type has size");

	unsigned char storage[sizeof(call_tracker)];
	error(type<call_tracker>().size != sizeof(storage), "size of call tracker incorrect");

	error(called_default, "called_default already set");
	type<call_tracker>().construct_default(storage);
	error(!called_default, "failed to call default constructor");

	error(called_destroy, "called_destroy already set");
	type<call_tracker>().destroy(storage);
	error(!called_destroy, "failed to call destructor");

	error(called_copy, "called_copy already set");
	type<call_tracker>().construct_copy(storage, storage);
	error(!called_copy, "failed to call copy constructor");

	error(called_assign, "called_assign already set");
	type<call_tracker>().assign(storage, storage);
	error(!called_assign, "failed to call assignment operator");

	type<call_tracker>().destroy(storage);


	stderr::line("type passed test");
}
