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
#endif
	using string = basic_string<char, char_traits<char>, allocator<char>>;
}

namespace library {

template <typename element_type> class heapvector;
template <typename element_type, size_t reserved> class stackvector;

class string
{
public:
	string();
	string(char const * source);
	string(string && source) : string(source.move()) {}
	string(string const & source) : string(source.std()) {}
	~string();

	// should provide a virtual function interface for compilation speed
	template <template<typename> class Container>
	string(Container<string> const & source, string join = "")
	: string()
	{
		bool continuing = false;
		for (auto & item : source) {
			if (continuing) {
				(*this) += join;
			} else {
				continuing = true;
			}
			(*this) += item;
		}
	}

	explicit string(bool);
	string(char); string(unsigned char);
	string(short); string(unsigned short);
	string(int); string(unsigned int);
	string(long); string(unsigned long);
	string(long long); string(unsigned long long);
	string(float); string(double); string(long double);
	string(void *);
	template <typename T>
	string(T * pointer) : string((void*)pointer) { }

	struct stringable
	{
		virtual string to_string() const = 0;
	};
	string(const stringable &);

	string & operator+=(string const & other);
	string & operator=(string const & other);
	char & operator [](size_t index);
	size_t size() const;
	char * data();
	char * begin();
	char * end();

	struct range {
		size_t begin;
		size_t end;
	};
	// first range of each result is whole match; rest are subexpressions
	heapvector<stackvector<range, 10>> regular_expression(string expression) const;

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
