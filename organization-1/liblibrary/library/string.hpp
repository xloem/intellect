#pragma once

#include <ciso646> // empty in c++ plus sets up glibcxx define

namespace std {
	template <class C> struct char_traits;
	template <class C> class allocator;
#if _GLIBCXX_USE_CXX11_ABI
	inline namespace __cxx11 {
#endif
	template <class C, class T, class A> class basic_string;
#if _GLIBCXX_USE_CXX11_ABI
	} // namespace __cxx11
	using namespace __cxx11;
#endif
	using string = basic_string<char, char_traits<char>, allocator<char>>;
}

#include <initializer_list>

namespace library {

template <typename> struct range;


class string
{
public:
	string();
	string(char * source);
	string(char const * source);
	string(string && source) : string(source.move()) {}
	string(string const & source) : string(source.std()) {}
	~string();

	// these are parsing-related.  make string a subclass of heapvector, if needed, but might make more sense to make converters.
	string(bool);
	string(char); string(unsigned char, int base = 16);
	string(short, int base = 10); string(unsigned short, int base = 10);
	string(int, int base = 10); string(unsigned int, int base = 10);
	string(long, int base = 10); string(unsigned long, int base = 10);
	string(long long, int base = 10); string(unsigned long long, int base = 10);
	string(float, int base = 10); string(double, int base = 10); string(long double, int base = 10);
	string(void *, int base = 16);
	template <typename T>
	string(T * pointer) : string((void*)pointer) { }

	// if we had some kind of virtual iterator this could be taken
	// out of header file
	template <template <typename> typename Container, typename element_type>
	string(Container<element_type> const & source, string join/*no default*/)
	: string()
	{
		bool continuing = false;
		for (auto const & item : source) {
			if (continuing) {
				(*this) += join;
			} else {
				continuing = true;
			}
			(*this) += string(item);
		}
	}
	template <typename element_type>
	string(std::initializer_list<element_type> const & source, string join/*no default*/)
	: string()
	{
		bool continuing = false;
		for (auto const & item : source) {
			if (continuing) {
				(*this) += join;
			} else {
				continuing = true;
			}
			(*this) += string(item);
		}
	}

	struct stringable
	{
		virtual string to_string() const = 0;
	};

	string(const stringable &);

	string & operator+=(string const & other);
	string & operator=(string const & other);
	char & operator [](unsigned long index);
	unsigned long size() const;

	library::range<char *> range();
	library::range<char const *> range() const;

	char * data(); char const * data() const;
	char * begin(); char const * begin() const;
	char * end(); char const * end() const;

	string to_string();

	void replace(library::range<char *>, string const & with);

	char const * c_str() const;

	string(std::string && source);
	string(std::string const & source);

	std::string & std();
	std::string const & std() const;
	std::string && move();

private:
	std::string * storage;
};

string operator+(string const & left, string const & right);
string operator,(string const & left, string const & right); // synonym for +
bool operator==(string const & left, string const & right);
bool operator!=(string const & left, string const & right);
bool operator<(string const & left, string const & right);
bool operator<=(string const & left, string const & right);
bool operator>(string const & left, string const & right);
bool operator>=(string const & left, string const & right);

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
