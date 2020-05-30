#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tools {

// a registered thing is added to a protected 'registry' set
// that can be used to enumerate all of them
template <typename Derived>
class registered
{
public:
	registered()
	{
		registry.insert((Derived*)this);
	}
	~registered()
	{
		registry.erase((Derived*)this);
	}

protected:
	static std::unordered_set<Derived*> registry;
};
template <typename Derived>
std::unordered_set<Derived*> registered<Derived>::registry;

// a place has things that are immediately near to it
template <typename Derived, typename Way>
class place
{
public:
	using reference = std::shared_ptr<Derived>;
	virtual std::vector<Way> near-ways() = 0;

	std::vector<Derived> near-places()
	{
		// stub
	}
};

template <typename Place>
class explore
{
public:
	explore(Place start)
	: queue({start})
	{ }

	Place next()
	{
		Place next = queue.back();
		queue.pop_back();
		for (Place place : next.near-places()) {
			queue.push_back(place);
		}
	}
protected:
	vector<Place> queue;
};

bool starts_with(std::string const & word, std::string const & start)
{
	return word.compare(0, start.size(), start) == 0;
}

char const * without_start(std::string const & word, std::string const & start)
{
	return word.c_str() + start.size();
}

std::string mutate_start_into_endfixes(std::string const & word, char const * word_prefix, char const * word_suffix, std::string const & start, std::string const & start_prefix, char const * start_suffix)
{
	if (starts_with(word, start)) {
		return start_prefix + without_start(word, start) + start_suffix;
	} else {
		return word_prefix + word + word_suffix;
	}
}

std::string quote_special(std::string const & word, bool quotes = true)
{
	return mutate_start_into_endfixes(word, quotes ? "'" : "", quotes ? "'" : "", "special-", "[", "]");
}

std::string line_word()
{
	// this part stores a line-expression, and provides the words
	// from the line in order.
	
	static std::istringstream words;
	static bool newline = true;

	if (newline) {
		std::string line;
		if (!std::getline(std::cin, line)) { return {}; }

		words.str(line);
		words.clear();
		newline = false;
	}
	
	std::string word;
	if (!(words >> word)) {
		newline = true;
		return {};
	}

	return word;
}

}
