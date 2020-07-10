#pragma once

namespace library {

class criu
{
public:
	criu();
	~criu();

	char const * dump(bool & restored, char const * label = "dump", char const * path = ".");
	void restore(char const * name = nullptr, char const * path = "."); // defaults to last dump

private:
	void * criu_opts;
};


} // namespace library
