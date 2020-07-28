#include <cctype>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>

#include <library/heapvector_definition.cpp>
#include <library/stackvector_definition.cpp>
#include <library/type_definition.cpp>
#include <library/string.hpp>

namespace library {

template class heapvector<string>;
template class stackvector<string>;

string::string()
: storage(new std::string())
{
	allocation_debugger.allocate(storage, type<std::string>());
}

string::string(char const * source, int size)
: storage(size ? new std::string(source, size) : new std::string(source))
{
	allocation_debugger.allocate(storage, type<std::string>());
}

string::string(char * source, int size)
: storage(size ? new std::string(source, size) : new std::string(source))
{
	allocation_debugger.allocate(storage, type<std::string>());
}

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
{
	allocation_debugger.access(&source, type<std::string>());
	storage = new std::string(std::move(source));
	allocation_debugger.allocate(storage, type<std::string>());
}

string::string(std::string const & source)
{
	allocation_debugger.access(&source, type<std::string>());
	storage = new std::string(source);
	allocation_debugger.allocate(storage, type<std::string>());
	/*
	recursion_detector<string> stack;
	if (!stack.depth()) {
		stderr::line("construct string at "_s + this + " using " + storage);
	}
	*/
}

string::~string()
{
	allocation_debugger.deallocate(storage, type<std::string>());
	/*
	recursion_detector<string> stack;
	if (!stack.depth()) {
		stderr::line("destroy string at "_s + this + " using " + storage);
	}
	*/
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

static string baseprefix(int base)
{
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
			result[0] = 'a' + base - 10;
		}
		return result;
	}
}

unsigned long prefixbaseoffset(char const * pointer, int & base)
{
	if (base == 0 && pointer[0] == '0' && pointer[1] >= '0' && pointer[1] < '8') {
		base = 8;
		return 1;
	} else if (pointer[0] == '0' && pointer[1] == 'b') {
		base = 2;
		return 2;
	} else if (pointer[0] == '0' && pointer[1] == 'd') {
		base = 10;
		return 2;
	} else if (pointer[0] == '0' && pointer[1] == 'x') {
		base = 16;
		return 2;
	} else if (pointer[1] == 'B' && pointer[0] >= '0' && pointer[0] <= '9') {
		base = pointer[0] - '0';
		return 2;
	} else if (pointer[1] == 'B' && pointer[0] >= 'a' && pointer[0] <= 'z') {
		base = pointer[0] - 'a' + 10;
		return 2;
	} else {
		return 0;
	}
}

/*
template <typename T>
static inline void real_to_chars(string & output, T value, int base, bool prefix, unsigned long precision)
{
	unsigned long offset = 0;
	if (prefix) {
		output = baseprefix(base);
		offset = output.size();
		output.resize(offset + 32);
	} else {
		output.resize(32);
	}

	std::to_chars_result result;
	if (base != 10 && base != 16) {
		throw std::invalid_argument("unimplemented floating point base");
	}
	if (precision) {
		result = std::to_chars(output.data() + offset, output.data() + output.size(), value, (base == 16) ? std::chars_format::hex : 0, precision);
	} else {
		result = std::to_chars(output.data() + offset, output.data() + output.size(), value, (base == 16) ? std::chars_format::hex : 0);
	}

	if (result.ptr != output.end()) {
		output.resize(result.ptr - output.begin());
	}
	
	// [below expression spawned in response to noting the responses
	//  to inhibition stemming from more-things-at-once]
	// we often make a mess in our attempt to organize.
	// maybe we do this more than we are aware of, because of
	// memory issues.
}
*/

template <typename T>
static inline void integer_to_chars(string & output, T value, int base, bool prefix, unsigned long digits)
{
	bool extend = digits;
	if (!extend) {
		digits = (value ? ((unsigned long)(log(value)/log(base))) : 0) + 1;
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
	if (offset && output[offset] == '-') {
		for (unsigned long pos = offset; pos > 0; -- pos) {
			output[pos] = output[pos - 1];
		}
		output[0] = '-';
	}
	
	// we often make a mess in our attempt to organize.
	// maybe we do this more than we are aware of, because of
	// memory issues.
}


string::string(void const * pointer, int base, bool prefix, int digits)
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

string::string(float real, int base, bool prefix, int precision)
: string((double)real, base, prefix, precision)
{ }

string::string(double real, int base, bool prefix, int precision)
: string()
{
	unsigned long length, offset;
	switch (base) {
	case 10:
		length = 1 + snprintf(data(), 0, "%.*f", precision, real);
		if (prefix) {
			resize(length + 2);
			(*this)[0] = '0';
			(*this)[1] = 'd';
			offset = 2;
		} else {
			resize(length);
			offset = 0;
		}
		snprintf(data() + offset, length, "%.*f", precision, real);
		break;
	case 16:
		length = 1 + snprintf(data(), 0, "%.*a", precision, real);
		resize(length);
		snprintf(data(), length, "%.*a", precision, real);
		// TODO: remove prefix if not requested
		break;
	default:
		throw std::invalid_argument("unimplemented floating point base");
	}
	resize(size() - 1);
}

string::string(long double real, int base, bool prefix, int precision)
: string()
{
	unsigned long length, offset;
	switch (base) {
	case 10:
		length = 1 + snprintf(data(), 0, "%.*Lf", precision, real);
		if (prefix) {
			resize(length + 2);
			(*this)[0] = '0';
			(*this)[1] = 'd';
			offset = 2;
		} else {
			resize(length);
			offset = 0;
		}
		snprintf(data() + offset, length, "%.*Lf", precision, real);
		break;
	case 16:
		length = 1 + snprintf(data(), 0, "%.*La", precision, real);
		resize(length);
		snprintf(data(), length, "%*.La", precision, real);
		// TODO: remove prefix if not requested
		break;
	default:
		throw std::invalid_argument("unimplemented floating point base");
	}
	resize(size() - 1);
}

string string::file(string filename)
{
	std::ifstream file(filename.std());
	std::ostringstream sstr;
	sstr << file.rdbuf();
	return sstr.str();
}

void string::lower()
{
	for (char & c : *this) {
		c = std::tolower(c);
	}
}

void string::upper()
{
	for (char & c : *this) {
		c = std::toupper(c);
	}
}

string string::lowered()
{
	string result = *this;
	result.lower();
	return result;
}

string string::uppered()
{
	string result = *this;
	result.upper();
	return result;
}

bool string::to_bool()
{
	string compare = lowered();
	if (compare == "true" || compare == "t" || compare == "yes" || compare == "y") {
		return true;
	}
	if (compare == "false" || compare == "f" || compare == "no" || compare == "n" || !compare.size()) {
		return false;
	}
	return to_signed_long_long();
}

char string::to_char()
{
	if (size() == 1) {
		return (*this)[0];
	} else if (size() > 1) {
		return to_signed_char(10);
	} else {
		return 0;
	}
}

void * string::to_pointer(int base)
{
	return (void*)to_unsigned_long(base);
}

signed char string::to_signed_char(int base)
{
	return to_signed_long(base);
}

unsigned char string::to_unsigned_char(int base)
{
	return to_unsigned_long(base);
}

signed short string::to_signed_short(int base)
{
	return to_signed_long(base);
}

unsigned short string::to_unsigned_short(int base)
{
	return to_unsigned_long(base);
}

signed int string::to_signed_int(int base)
{
	return to_signed_long(base);
}

unsigned int string::to_unsigned_int(int base)
{
	return to_unsigned_long(base);
}

signed long string::to_signed_long(int base)
{
	bool negative = false;
	char const * data = c_str();
	if (data[0] == '-') {
		negative = true;
		++ data;
	}
	data += prefixbaseoffset(data, base);
	return negative ? -std::strtol(data, 0, base) : std::strtol(data, 0, base);
}

unsigned long string::to_unsigned_long(int base)
{
	char const * data = c_str();
	data += prefixbaseoffset(data, base);
	return std::strtoul(data, 0, base);
}

signed long long string::to_signed_long_long(int base)
{
	bool negative = false;
	char const * data = c_str();
	if (data[0] == '-') {
		negative = true;
		++ data;
	}
	data += prefixbaseoffset(data, base);
	return negative ? -std::strtoll(data, 0, base) : std::strtoll(data, 0, base);
}

unsigned long long string::to_unsigned_long_long(int base)
{
	char const * data = c_str();
	data += prefixbaseoffset(data, base);
	return std::strtoll(data, 0, base);
}

float string::to_float(int base)
{
	if (base != 10 && base != 16) { throw std::invalid_argument("unimplemented floating point base"); }
	char const * data = c_str();
	bool negative = false;
	if (data[0] == '-') {
		++ data;
		negative = true;
	}
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	return negative ? -std::strtof(data, 0) : std::strtof(data, 0);
}

double string::to_double(int base)
{
	if (base != 10 && base != 16) { throw std::invalid_argument("unimplemented floating point base"); }
	char const * data = c_str();
	bool negative = false;
	if (data[0] == '-') {
		++ data;
		negative = true;
	}
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	return negative ? -std::strtod(data, 0) : std::strtod(data, 0);
}

long double string::to_long_double(int base)
{
	if (base != 10 && base != 16) { throw std::invalid_argument("unimplemented floating point base"); }
	char const * data = c_str();
	bool negative = false;
	if (data[0] == '-') {
		++ data;
		negative = true;
	}
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	return negative ? -std::strtold(data, 0) : std::strtold(data, 0);
}

void string::to_file(string filename)
{
	std::ofstream file(filename.std());
	std::istringstream sstr(std());
	file << sstr.rdbuf();
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

heapvector<string> string::split(string delimiter)
{
	heapvector<string> result;
	size_t last = 0;
	while (last < size()) {
		size_t next = std().find(delimiter.std(), last);
		if (next == std::string::npos) { next = size(); }
		result.push_back(std().substr(last, next - last));
		last = next + 1;
	}
	return result;
}

char const * string::c_str() const
{
	return storage->data();
}

std::string & string::std()
{
	return *storage;
}

string::operator std::string &()
{
	return std();
}

std::string const & string::std() const
{
	return *storage;
}

string::operator std::string const &() const
{
	return std();
}

std::string && string::move()
{
	return std::move(*storage);
}

string::operator std::string &&()
{
	return move();
}

string operator "" _s(char const source [], unsigned long size)
{
	return {source, (int)size};
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
