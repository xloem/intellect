#pragma once

// puzzle has knobs to turn to change
// knobs for growing parts
// and knobs for the parts
//
template <typename Part, typename BigShapes, typename SmallShapes>
class puzzle
{
public:
	virtual std::string name();
	virtual std::pair<size_t,size_t> number();
	virtual SmallShapes guess(size_t part);
	virtual place(BigShapes shape);

	std::vector<std::shared_ptr<Part>> parts;
};

// puzzles of puzzles guess configurations of their parts
// puzzles of numbers guess straight numbers

#include "three_way_number_puzzle.hpp"

// let's make a puzzle for building a number puzzle
// let's just work it more
//

template <typename Number>
class number_puzzle : public puzzle<three_way_number_puzzle<Number>, silly, Number>
{
public:
	void consider_adding_part(three_way_number_puzzle & part) { /*STUB*/ }
			// we a possible smooth puzzle piece
			//
	// what is smooth puzzle piece
		// it considers multiple options
		// and it can can consider them more or less
};



	// can you break the ideas up into little bundles?


// this could be a smooth puzzle.
// but its parts are too big.
// good parts are maybe 11 code lines.  1-2 1-line functions.

// maybe below is silly.
// can we generalize selecting options?
// seems like it has three parts relevant, build-up-interest, try-doing, and feedback
// 	smooth-puzzle has try-doing, but not feedback-association
// 		when you try-doing, you try with specific interest
// 			[that is context of smoothness]
// 		so ... context maps to try-event.
// 		build-up-interest moves through context.  it is like, where will i put my smooth puzzle part?

// when considering options, we smoothly move between interest in the options
// when we try an option, we are considering that option in that context of interest.
// 	this probably relates to the puzzle of doing puzzles better than before.
// 		[what a big context]
// 	but basically, this was a choice, and its context can be captured from when it was made.
// 	it shows that context is made of many values.

template <typename Key, typename Value>
class ordered_association
{
public:
	// todo: brackets
	void set(Key const & key, Value const & value)
	{ 
		// nothing wrong with continuing here.  can we close concern? or are there more?
		// 	this is just a puzzle so we're thinking of solving the goal with more thinking rather than handling this concern-situation.
		if (by_value.contains
		by_key[key] = value;
	}
	// usually c++ parts are big and huge with many features that make them small and compact to use.
	// it makes sense to do that later, after design is set out.
	std::map<Key, Value> by_key;
	std::multimap<Value, Key> by_value;
}

template <typename Value, typename Option>
class smooth_considering_kind
{
public:
	// this considering-kind is a smooth puzzle I think?
	void consider(Option option, Value degree)
	{
		interest[option] += degree;
	}

	Option pick() // might be too many lines
	{
		// this is long because we don't have a structure that stays sorted when changed
		// smooth-puzzles is asking for refinedness to ease thinking-about
		// 	[i don't have a way to close-process aroudn the concern about why <- in resolution]
		auto best = interest.begin();
		for (auto option = best; ; (++ option) != interest.end())
		{
			if (option->second > best->second) {
				best = option;
			}
		}
		return best->first;
	}

	// does it update around guesses?
	// it looks our very interest, our considering, is a guess.
	// and it looks like we can build thatn guess slowly!

	std::map<Option, Value> interest;
	std::map<Value, Option> options;
};
































	// this is probably a smooth puzzle, although it is less smooth.
	// smooth puzzles hold guesses.  those are probably the values of our
	// parts.
	// thfgen they update to update the guesses.
	// that's the smoothness.
	// when a threeway updates, figures out two new parts, and updates those.
