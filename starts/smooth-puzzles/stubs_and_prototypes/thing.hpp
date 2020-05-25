#pragma once

#include <memory>
#include <string>
#include <unordered_map>

using name = std::string;

class thing {
public:
	std::shared_ptr<std::any> what;

	template <typename T>
	bool is() { throw "stub"; }

	std::unordered_map<name,thing> stuff;
};
