#pragma once

// This library reproduces existing work in slightly-different ways.
// It is so hard to reach the existing work, and work with it.


#include <ciso646> // empty in c++ plus sets up glibcxx define

// forward declaration for std::string, provided by interface
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
	string(char * source, int size = 0);
	string(char const * source, int size = 0);
	string(string && source) : string(source.move()) {}
	string(string const & source) : string(source.std()) {}
	~string();

	// these are parsing-related.  make string a subclass of heapvector, if needed, but might make more sense to make converters.
	string(bool); // converts to "true" or "false"
	string(char); // makes a size=1 string
	string(            void *, int base = 16, bool prefix = true,  int digits = sizeof(void*)*2);
	string(       signed char, int base,      bool prefix = false, int digits = 0);
	string(     unsigned char, int base = 16, bool prefix = false, int digits = sizeof(unsigned char)*2);
	string(      signed short, int base = 10, bool prefix = false, int digits = 0);
	string(    unsigned short, int base = 10, bool prefix = false, int digits = 0);
	string(        signed int, int base = 10, bool prefix = false, int digits = 0);
	string(      unsigned int, int base = 10, bool prefix = false, int digits = 0);
	string(       signed long, int base = 10, bool prefix = false, int digits = 0);
	string(     unsigned long, int base = 10, bool prefix = false, int digits = 0);
	string(  signed long long, int base = 10, bool prefix = false, int digits = 0);
	string(unsigned long long, int base = 10, bool prefix = false, int digits = 0);
	string(      float, int base = 10, bool prefix = false, int precision = -1);
	string(     double, int base = 10, bool prefix = false, int precision = -1);
	string(long double, int base = 10, bool prefix = false, int precision = -1);

	// this is getting called for (char const *, size)
	// if we made prefix be second argument could be quick fix
	//template <typename T>
	//string(T * pointer, int base = 16, bool prefix = true, int digits = sizeof(T*)*2)
	//: string((void*)pointer, base, prefix, digits)
	//{ }

	bool to_bool();
	char to_char();
	void * to_pointer(int base = 16);
	signed char        to_signed_char(int base); // for now, base is required because the unsigned char constructor assumes an abnormal base of 16
	unsigned char      to_unsigned_char(int base);          // any other solutions to ease and error-prevention?
	signed short       to_signed_short(int base = 10);
	unsigned short     to_unsigned_short(int base = 10);
	signed int         to_signed_int(int base = 10);
	unsigned int       to_unsigned_int(int base = 10);
	signed long        to_signed_long(int base = 10);
	unsigned long      to_unsigned_long(int base = 10);
	signed long long   to_signed_long_long(int base = 10);
	unsigned long long to_unsigned_long_long(int base = 10);
	float       to_float(int base = 10);
	double      to_double(int base = 10);
	long double to_long_double(int base = 10);

	void lower();
	string lowered();

	void upper();
	string uppered();


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
	char const & operator [](unsigned long index) const;
	unsigned long size() const;
	void resize(unsigned long new_size);

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
