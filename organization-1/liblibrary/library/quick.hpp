#pragma once

#include <library/string.hpp>
#include <library/heapvector.hpp>

namespace library {

// if these were made members of library::string, then they could stay non-library::string-dependent over here
string file_read(string filename);
void file_write(string filename, string data);

template <typename T>
void to_number(char const * text, T & number, int base = 10);

heapvector<string> split(string text, string delimiter);

}
