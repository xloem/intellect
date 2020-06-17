#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <library/string.hpp>

namespace library {

string::string()
: storage(new std::string())
{ }

string::string(char const * source)
: storage(new std::string(source))
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
: string(" ")
{
	(*this)[0] = character;
}

string::string(unsigned char byte)
: string()
{
	std::stringstream stream;
	stream << std::hex << std::setfill('0') << std::setw(sizeof(byte) * 2);
	stream << (int)byte;
	std() = stream.str();
}

string::string(short integer)
: string(std::to_string(integer))
{ }

string::string(unsigned short integer)
: string(std::to_string(integer))
{ }

string::string(int integer)
: string(std::to_string(integer))
{ }

string::string(unsigned int integer)
: string(std::to_string(integer))
{ }

string::string(long integer)
: string(std::to_string(integer))
{ }

string::string(unsigned long integer)
: string(std::to_string(integer))
{ }

string::string(long long integer)
: string(std::to_string(integer))
{ }

string::string(unsigned long long integer)
: string(std::to_string(integer))
{ }

string::string(float real)
: string(std::to_string(real))
{ }

string::string(double real)
: string(std::to_string(real))
{ }

string::string(long double real)
: string(std::to_string(real))
{ }

string::string(void* pointer)
: string()
{
	std::stringstream stream;
	stream << pointer;
	std() = stream.str();
}

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

unsigned long string::size() const
{
	return storage->size();
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
