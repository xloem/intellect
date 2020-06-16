#include <library/string.hpp>

#include <string>
#include <iostream>

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

string::string(long long integer)
: string(std::to_string(integer))
{ }

string::string(double real)
: string(std::to_string(real))
{ }

string::string(void* pointer)
: string(std::to_string(pointer))
{ }

string string::operator+(string const & other)
{
	return {std() + other.std()};
}

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

char & string::operator[](size_t index)
{
	return (*storage)[index];
}

size_t string::size()
{
	return storage->size();
}

char * string::data()
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
