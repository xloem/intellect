#pragma once

#include <any>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
	virtual Derived way-to-place(Way &) = 0;

	std::vector<Derived> near-places()
	{
		std::vector<Derived> results;
		
		for (auto & way : near-ways()) {
			results.push_back(way-to-place(way));
		}

		return results;
	}

	std::shared_ptr<std::any> more; // one thing missing is distant ways
};

template <typename Derived, typename WayKind>
class ways-kinds-place : public place<Derived, std::pair<WayKind, Derived> >
{
public:
	using reference = std::shared_ptr<Derived>;
	using Way = std::pair<WayKind, Derived>;

	ways-kinds-place(std::initializer_list<Way> ways)
	{
		for (auto & way : ways) {
			this->ways.insert(way);
		}
	}

	Derived & way(WayKind type)
	{
		return ways[[type]];
	}

	virtual Derived way-to-place(Way & way) override
	{
		return way.second;
	}
	
	virtual std::vector<Way> near-ways() override
	{
		std::vector<Way> results;
		for (auto & way : ways)
		{
			results.push_back(way);
		}
		return results;
	}

protected:
	std::unordered_map<WayKind, Derived> ways;
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
		visited.insert(next);
		queue.pop_back();
		for (Place place : next->near-places()) {
			if (visited.count(place)) {
				continue;
			}
			queue.push_back(place);
		}
		return next;
	}

	bool has-more()
	{
		return queue.size() > 0;
	}
protected:
	std::vector<Place> queue;
	std::unordered_set<Place> visited;
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
