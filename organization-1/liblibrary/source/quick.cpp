#include <library/quick.hpp>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string file_read(string filename)
{
	ifstream file(filename);
	ostringstream sstr;
	sstr << file.rdbuf();
	return sstr.str();
}

void file_write(string filename, string data)
{
	ofstream file(filename);
	istringstream sstr(data);
	file << sstr.rdbuf();
}

vector<string> split(string text, string delimiter)
{
	vector<string> result;
	size_t last = 0;
	while (last < text.size()) {
		size_t next = text.find(delimiter, last);
		if (next == string::npos) { next = text.size(); }
		result.push_back(text.substr(last, next - last));
		last = next + 1;
	}
	return result;
}

template <>
void to_number<int>(string data, int & number, int base)
{
	number = std::stoi(data, 0, base);
}	

template <>
void to_number<long>(string data, long & number, int base)
{
	number = std::stol(data, 0, base);
}	

template <>
void to_number<long long>(string data, long long & number, int base)
{
	number = std::stoll(data, 0, base);
}	

template <>
void to_number<unsigned long>(string data, unsigned long & number, int base)
{
	number = std::stoul(data, 0, base);
}	

template <>
void to_number<unsigned long long>(string data, unsigned long long & number, int base)
{
	number = std::stoull(data, 0, base);
}

template <>
void to_number<float>(string data, float & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stof(data, 0);
}

template <>
void to_number<double>(string data, double & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stod(data, 0);
}

template <>
void to_number<long double>(string data, long double & number, int base)
{
	if (base != 10) { throw std::invalid_argument("unimplemented floating point base"); }
	number = std::stold(data, 0);
}
