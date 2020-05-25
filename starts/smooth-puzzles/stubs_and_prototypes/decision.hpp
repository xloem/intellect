#pragma

// a decision of course is a single choice
// the developing memory around what good choices are might have a better name

#include "codeways"

// a decision is a something or other tha learns an option
template <typename Option, typename Number>
class decision
{
public:
	void consider(Option option, Number amount)
	{
		// modern compilers will automatically create 0-valued options
		options[option] += amount;
	}

	Option pick()
	{

		auto best = std::max_element(map.begin(), map.end(), [](Iterator a, Iterator b) { return a->second > b->second; });
		return best->first;
		return codeways::best(options)->first;
	}
private:
	std::map<Option, Number> options;
	using Iterator = decltype(options)::iterator;
};
