#pragma once

#include <string>
#include <vector>

std::string file_read(std::string filename);
void file_write(std::string data, std::string filename);

template <typename T>
void to_number(std::string text, T & number, int base = 10);

std::vector<std::string> split(std::string text, std::string delimiter);
