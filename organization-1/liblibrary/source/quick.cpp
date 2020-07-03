#include <library/quick.hpp>
#include <library/string.hpp>

#include <library/heapvector_definition.cpp>

#include <fstream>
#include <sstream>

namespace library {

template class heapvector<string>;

string file_read(string filename)
{
	std::ifstream file(filename.std());
	std::ostringstream sstr;
	sstr << file.rdbuf();
	return sstr.str();
}

void file_write(string filename, string data)
{
	std::ofstream file(filename.std());
	std::istringstream sstr(data.std());
	file << sstr.rdbuf();
}

heapvector<string> split(string _text, string delimiter)
{
	auto text = _text.std();
	heapvector<string> result;
	size_t last = 0;
	while (last < text.size()) {
		size_t next = text.find(delimiter.std(), last);
		if (next == std::string::npos) { next = text.size(); }
		result.push_back(text.substr(last, next - last));
		last = next + 1;
	}
	return result;
}

void pointer_and_base_to_integer_prefix(char const * & pointer, int & base)
{
	if (base == 0 && pointer[0] == '0' && pointer[1] >= '0' && pointer[1] < '8') {
		base = 8;
		++ pointer;
	} else if (pointer[0] == '0' && pointer[1] == 'b') {
		base = 2;
		pointer += 2;
	} else if (pointer[0] == '0' && pointer[1] == 'd') {
		base = 10;
		pointer += 2;
	} else if (pointer[0] == '0' && pointer[1] == 'x') {
		base = 16;
		pointer += 2;
	} else if (pointer[1] == 'B' && pointer[0] >= '0' && pointer[0] <= '9') {
		base = pointer[0] - '0';
		pointer += 2;
	} else if (pointer[1] == 'B' && pointer[0] >= 'a' && pointer[0] <= 'z') {
		base = pointer[0] - 'a' + 10;
		pointer += 2;
	}
}

template <>
void to_number<int>(char const * data, int & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtol(data, 0, base);
}	

template <>
void to_number<long>(char const * data, long & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtol(data, 0, base);
}	

template <>
void to_number<long long>(char const * data, long long & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtoll(data, 0, base);
}	

template <>
void to_number<unsigned int>(char const * data, unsigned int & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtoul(data, 0, base);
}	

template <>
void to_number<unsigned long>(char const * data, unsigned long & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtoul(data, 0, base);
}	

template <>
void to_number<unsigned long long>(char const * data, unsigned long long & number, int base)
{
	pointer_and_base_to_integer_prefix(data, base);
	number = std::strtoull(data, 0, base);
}

template <>
void to_number<float>(char const * data, float & number, int base)
{
	if (base != 10 && base != 16) { throw std::invalid_argument("unimplemented floating point base"); }
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	number = std::strtof(data, 0);
}

template <>
void to_number<double>(char const * data, double & number, int base)
{
	if (base != 10 && base != 16) { throw std::invalid_argument("unimplemented floating point base"); }
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	number = std::strtod(data, 0);
}

template <>
void to_number<long double>(char const * data, long double & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	if (data[0] == '0' && data[1] == 'd') {
		data += 2;
		base = 10;
	}
	number = std::strtold(data, 0);
}

}
