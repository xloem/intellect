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

template <>
void to_number<int>(string data, int & number, int base)
{
	number = std::stoi(data.std(), 0, base);
}	

template <>
void to_number<long>(string data, long & number, int base)
{
	number = std::stol(data.std(), 0, base);
}	

template <>
void to_number<long long>(string data, long long & number, int base)
{
	number = std::stoll(data.std(), 0, base);
}	

template <>
void to_number<unsigned long>(string data, unsigned long & number, int base)
{
	number = std::stoul(data.std(), 0, base);
}	

template <>
void to_number<unsigned long long>(string data, unsigned long long & number, int base)
{
	number = std::stoull(data.std(), 0, base);
}

template <>
void to_number<float>(string data, float & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stof(data.std(), 0);
}

template <>
void to_number<double>(string data, double & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stod(data.std(), 0);
}

template <>
void to_number<long double>(string data, long double & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stold(data.std(), 0);
}

}
