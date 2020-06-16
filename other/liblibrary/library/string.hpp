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
	string(long long);
	string(double);
	string(void*);

	template <typename T> string(T const & object)
	: string(object.to_string()) { }

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
