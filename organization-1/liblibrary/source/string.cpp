#include <charconv>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>

#include <library/string.hpp>

namespace library {

string::string()
: storage(new std::string())
{ }

string::string(char const * source, int size)
: storage(size ? new std::string(source, size) : new std::string(source))
{ }

string::string(char * source, int size)
: storage(size ? new std::string(source, size) : new std::string(source))
{ }

/*
template <template<typename> class Container>
string::string<Container>(Container<string> const & source)
: string()
{
	for (auto & item : source)
	{
		(*this) += item;
	}
}
*/

string::string(std::string && source)
: storage(new std::string(std::move(source)))
{ }

string::string(std::string const & source)
: storage(new std::string(source))
{ }

string::~string()
{
	delete storage;
}

string::string(bool flag)
: string(flag ? "true" : "false")
{ }

string::string(char character)
: string(&character, 1)
{ }

auto streambase(int base)
{
	switch (base) {
	case 16:
		return std::hex;
	case 10:
		return std::dec;
	case 8:
		return std::oct;
	default:
		throw std::invalid_argument("unimplemented floating point base");
	}
}

static string baseprefix(int base) {
	switch(base) {
	case 8:
		return '0';
	case 2:
		return {"0b",2};
	case 10:
		return {"0d",2};
	case 16:
		return {"0x",2};
	default:
		string result(" B", 2);
		if (base < 10) {
			result[0] = '0' + base;
		} else {
			result[0] = 'a' + base;
		}
		return result;
	}
}

template <typename T>
static inline void integer_to_chars(string & output, T value, int base, bool prefix, unsigned long digits)
{
	bool extend = digits;
	if (!extend) {
		digits = ((unsigned long)(log(value)/log(base))) + 1;
	}
	unsigned long offset = 0;

	if (prefix) {
		output = baseprefix(base);
		offset = output.size();
		output.resize(offset + digits);
	} else {
		output.resize(digits);
	}

	auto result = std::to_chars(output.data() + offset, output.data() + output.size(), value, base);

	if (result.ptr != output.end()) {
		if (extend) {
			unsigned long difference = output.end() - result.ptr;
			char * start = &output[offset];
			memmove(start + difference, start, result.ptr - start);
			memset(start, '0', difference);
		} else {
			output.resize(result.ptr - output.begin());
		}
	}
	
	// we often make a mess in our attempt to organize.
	// maybe we do this more than we are aware of, because of
	// memory issues.
}

string::string(void* pointer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, (unsigned long)pointer, base, prefix, digits);
}

string::string(unsigned char byte, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, byte, base, prefix, digits);
}

string::string(signed short integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(unsigned short integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(signed int integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(unsigned int integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(signed long integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(unsigned long integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(signed long long integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(unsigned long long integer, int base, bool prefix, int digits)
: string()
{
	integer_to_chars(*this, integer, base, prefix, digits);
}

string::string(float real, int base, bool prefix, int digits)
: string()
{
	throw "unimplemented oops";
	//to_chars(*this, real, base, prefix, digits);
}

string::string(double real, int base, bool prefix, int digits)
: string()
{
	throw "unimplemented oops";
	//to_chars(*this, real, base, prefix, digits);
}

string::string(long double real, int base, bool prefix, int digits)
: string()
{
	throw "unimplemented oops";
	//to_chars(*this, real, base, prefix, digits);
}

// is todo-list making a mess?
// 	feels a little like one-big-thing-unaddressed
// 		all your todo items
// 		assume there is no mind control or global ai
// 		they don't understand the needs of handling these things

// 	ideas: -> let's plan for the time rather than using an alarm
// 		-> let's visit the todo list on a schedule, too.
// 		-> let's move toothbrushing _separate_ from them,
// 		   to grow fluid-learning a little

	// active memory dump for task changing:
	// 1. debug liblibrary so teststring works.
	//    verify teststring compiles in .1 to .2 seconds (>.5 is needs-understanding)
	// 2. implement regular expression parser in strings, very simple.
	//
	// [3. switch node4/preprocess.bash to use liblibrary's parser
	//     and node4's partially-preprocessed-methods
	//     		[note: we could lose ability to bootstrap,
	//     		 or errors cuold be hard to hunt down from bootstrappers.
	//     		 do not use parser when implementing parsser. still use
	//     		 node4. [no this breaks it]]
	//     [our active task has solvable recursive logic EXCITEMENT!
	//      first eye lubricant.]
	//

string::string(string::stringable const & object)
: string(object.to_string())
{ }

string & string::operator+=(string const & other)
{
	std() += other.std();
	return *this;
}

string & string::operator=(string const & other)
{
	std() = other.std();
	return *this;
}

char & string::operator[](unsigned long index)
{
	return (*storage)[index];
}

char const & string::operator[](unsigned long index) const
{
	return (*storage)[index];
}

unsigned long string::size() const
{
	return storage->size();
}

void string::resize(unsigned long size)
{
	storage->resize(size);
}

char * string::data()
{
	return storage->data();
}

char * string::begin()
{
	return storage->data();
}

char * string::end()
{
	return storage->data() + size();
}

char const * string::c_str() const
{
	return storage->data();
}

std::string & string::std()
{
	return *storage;
}

std::string const & string::std() const
{
	return *storage;
}

std::string && string::move()
{
	return std::move(*storage);
}

string operator+(string const & left, string const & right)
{
	return {left.std() + right.std()};
}

string operator,(string const & left, string const & right)
{
	return left + right;
}

bool operator==(string const & left, string const & right)
{
	return left.std() == right.std();
}

bool operator!=(string const & left, string const & right)
{
	return left.std() != right.std();
}

bool operator<(string const & left, string const & right)
{
	return left.std() < right.std();
}

bool operator<=(string const & left, string const & right)
{
	return left.std() <= right.std();
}

bool operator>(string const & left, string const & right)
{
	return left.std() > right.std();
}

bool operator>=(string const & left, string const & right)
{
	return left.std() >= right.std();
}

namespace stdin {
	string word()
	{
		string result;
		std::cin >> result.std();
		return result;
	}
	string line()
	{
		string result;
		std::getline(std::cin, result.std());
		return result;
	}
}
namespace stdout {
	void write(string data)
	{
		std::cout << data.std();
	}
	void line(string data)
	{
		std::cout << data.std() << std::endl;
	}
}
namespace stderr {
	void write(string data)
	{
		std::cerr << data.std();
	}
	void line(string data)
	{
		std::cerr << data.std() << std::endl;
	}
}

} // namespace library
