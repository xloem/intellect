#pragma once

#include <cstddef>

namespace std {
	template <class C> struct char_traits;
	template <class C> class allocator;
#if _GLIBCXX_USE_CXX11_ABI
	namespace __cxx11 {
#endif
	template <class C, class T, class A> class basic_string;
#if _GLIBCXX_USE_CXX11_ABI
	} // namespace __cxx11
	using namespace __cxx11;
	using string = basic_string<char, char_traits<char>, allocator<char>>;
#endif
}

namespace library {

class string
{
public:
	string();
	string(char const * source);
	string(string && source) : string(source.move()) {}
	string(string const & source) : string(source.std()) {}
	~string();

	// input
	static string in_word();
	static string in_line();
	// output, optionally with linebreak
	void err();
	void out();
	void errl();
	void outl();

	string operator+(string const & other);
	string & operator+=(string const & other);
	string & operator=(string const & other);
	char & operator [](size_t index);
	size_t size();
	char * data();

	char const * c_str();

	string(std::string && source);
	string(std::string const & source);

	std::string & std();
	std::string const & std() const;
	std::string && move();

private:
	std::string * storage;
};

namespace stdin {
	string word();
	string line();
}
namespace stdout {
	void write(string data);
	void line(string data);
}
namespace stderr {
	void write(string data);
	void line(string data);
}

} // namespace library
