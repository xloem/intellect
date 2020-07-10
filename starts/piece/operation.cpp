#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <cmath>
#include <memory>

// multiple-ways

template <typename T>
struct named_function : public std::function<T>
{
	template <typename T2>
	named_function(char const * name, T2 && t)
	: std::function<T>(std::forward<T2>(t)), name(name) { }

	char const * const name;
};

using binary_math = named_function<double(double,double)>;

std::vector<binary_math> operations = {
	{"+", [](double a, double b) -> double { return a + b; } },
	{"-", [](double a, double b) -> double { return a - b; } },
	//{"-2", [](double a, double b) -> double { return b - a; } },
	{"*", [](double a, double b) -> double { return a * b; } },
	{"/", [](double a, double b) -> double { return a / b; } },
	//{"/2", [](double a, double b) -> double { return b / a; } },
	{"pow", [](double a, double b) -> double { return std::pow(a,b); } },
	//{"pow2", [](double a, double b) -> double { return pow(b,a); } },
	{"log", [](double a, double b) -> double { return std::log(a)/std::log(b); } },
	//{"log2", [](double a, double b) -> double { return ln(b)/ln(a); } },
};

// we need some way of selecting values and using them as variables
// 	variables can be pointers, values, referneces, or objects wrapping something
// 	we pass a pointer in: we need a collection of functions it works with
//
// 		

// let us use expressions?
// 	let us use a list of functions writing to intermediate variables?
// 		so, make a function: then can make function on result if want.
// 		each new call can make a new variable

// incrementation
// or
// form a set of all options
//
// many-options, inner product

// chain is formed of pairs of iterators

template <typename... Types>
struct tuple_space
{
	struct iterator {
		using difference_type = void;
		using value_type = std::tuple<>;
		using pointer_type = value_type const *;
		using reference_type = value_type const &;
		using iterator_category = std::forward_iterator_tag;
		std::tuple<> operator*() const
		{
			return {};
		}
		bool operator++() const
		{
			return false;
		}
		bool operator==(iterator const & other) const
		{
			return true;
		}
		bool operator!=(iterator const & other) const
		{
			return false;
		}
	};

	iterator begin() const { return {}; }
	iterator end() const { return {}; }
};

template <typename Type, typename... Types>
struct tuple_space<Type, Types...>
{
	using tuple = std::tuple<Type, Types...>;
	tuple_space(Type begin, Types... nextbegin, Type end, Types... nextend)
	: begin_value(begin),
	  end_value(end),
	  next(nextbegin..., nextend...)
	{ }

	Type begin_value; Type end_value;
	tuple_space<Types...> next;

	struct iterator {
		tuple_space<Type, Types...> const & space;
		Type value;
		typename tuple_space<Types...>::iterator inner_iterator;

		using difference_type = void;
		using value_type = std::tuple<Type, Types...>;
		using pointer_type = value_type const *;
		using reference_type = value_type const &;
		using iterator_category = std::forward_iterator_tag;

		value_type operator*() const
		{
			return std::tuple_cat(std::make_tuple(value), *inner_iterator);
		}

		bool operator++()
		{
			// we could call subclass ++, and hold a tuple
			++ value;
			if (value == space.end_value) {
				if (!++inner_iterator) {
					return false;
				}
				value = space.begin_value;
			}
			return true;
		}

		bool operator==(iterator const & other) const
		{
			return value == other.value && inner_iterator == other.inner_iterator;
		}

		bool operator!=(iterator const & other) const
		{
			return value != other.value || inner_iterator != other.inner_iterator;
		}
	};
	
	iterator begin() const
	{
		iterator result = { *this, begin_value, next.begin() };
		return result;
	}

	iterator end() const
	{
		iterator result = { *this, end_value, next.end() };
		return result;
	}

};

template <typename Type>
class vector_space
{
public:
	using bounds_type = std::pair<std::vector<Type>,std::vector<Type>>;
	vector_space(long count, Type begin, Type end)
	{
		bounds = std::make_shared<bounds_type>();
		bounds->first.resize(count, begin);
		bounds->second.resize(count, end);
	}

	vector_space()
	{
		bounds = std::make_shared<bounds_type>();
	}

	long size()
	{
		return bounds->first.size();
	}

	void push(Type begin, Type end)
	{
		bounds->first.push_back(begin);
		bounds->second.push_back(end);
	}

	struct iterator {
		std::shared_ptr<std::pair<std::vector<Type>,std::vector<Type>>> space;
		std::vector<Type> values;

		using difference_type = void;
		using value_type = std::vector<Type>;
		using pointer = value_type const *;
		using reference = value_type const &;
		using iterator_category = std::forward_iterator_tag;

		reference operator*() const
		{
			return values;
		}
		void operator++()
		{
			for (long i = 0; i < space->second.size(); ++ i) {
				++ values[i];
				if (values[i] < space->second[i]) {
					for (-- i; i >= 0; -- i) {
						values[i] = space->first[i];
					}
					break;
				}
			}
		}
		bool operator==(iterator const & other) const
		{
			return space == other.space && values == other.values;
			/*if (
			for (long i = 0; i < space.size(); ++ i) {

			}*/
		}
		bool operator!=(iterator const & other) const
		{
			return !operator==(other);
		}
	};

	iterator begin() const { return {bounds, bounds->first}; }
	iterator end() const { return {bounds, bounds->second}; }

private:
	std::shared_ptr<bounds_type> bounds;
};

// ?
// tuple space iterates from begin to end of type
// vector space iterators from begin to end, two vectors
// tuple space wants begin and end to iterate through
// it will be taking an iterator, not a space

struct calls
{
	double inputs[2]; double values[30];
	static long const input_count = 2;;
	long value_count;

	struct function_call
	{
		long inputs[2];
		binary_math * function;
	};

	function_call functions[30];
	long function_count;

	using use_space = tuple_space<decltype(operations.begin()), vector_space<long>::iterator>;
	using use = use_space::tuple;

	static use_space get_use_space(long function_index)
	{
		vector_space<long> vectors(2, 0, function_index + input_count);
		return use_space(operations.begin(), vectors.begin(), operations.end(), vectors.end());
	}

	void set_inputs(double a, double b)
	{
		inputs[0] = a;
		inputs[1] = b;
	}

	long size() { return function_count; }

	double one_step(long index)
	{
		auto & function = functions[index];
		return values[index] = (*function.function)(inputs[function.inputs[0]], inputs[function.inputs[1]]);
	}

	double value()
	{
		return values[value_count - 1];
	}

	double call(double a, double b)
	{
		set_inputs(a, b);
		for (long i = 0; i < size(); ++ i) {
			one_step(i);
		}
		return value();
	}

	std::string to_string()
	{
		std::string result = "(a,b){";
		std::string expr;
		long i;
		for (i = 0; /*i < function_count*/; ++ i) {
			std::string vara = std::string(1, 'a' + functions[i].inputs[0]);
			std::string varb = std::string(1, 'a' + functions[i].inputs[1]);
			expr = vara + functions[i].function->name + varb + ";";
			if (i + 1 < function_count) {
				result += std::string(1, 'a' + i + input_count) + "=" + expr;
			} else {
				return result + "return " + expr + "}";
			}
		}
	}

	void set(use const & to, long function_index)
	{
		function_call * function = &functions[function_index];
		function->function = &*std::get<0>(to);
		function->inputs[0] = (*std::get<1>(to))[0];
		function->inputs[1] = (*std::get<1>(to))[1];
	}

	void push()
	{
		//function_call * function = &functions[function_count];
		//function->output = &values[value_count];
		++ function_count;
		++ value_count;
	}

	void push(use const & to)
	{ 
		set(to, function_count);
		push();
	}

	void pop()
	{
		-- function_count;
		-- value_count;
	}
};

// ok calls will make an exploration tree
// let's preserve all the different branches
// but let's also do it depth-first?
// 	let's do it breadth-first in parallel?
// hmmmmmmmmmmmmm ok

// breadth-first parallel structure: enumerate all options.
// try all of them.
// continue.

int main(int argc, char const ** argv)
{
	calls trial;

	std::vector<std::vector<calls::use>> uses;
	using use_iterator = std::vector<calls::use>::iterator;

	//std::vector<std::vector<calls::use>::iterator> use;


		// desired trial behavior:
		// 	set first options
		// 	try all of last option
		// 	loop
	
	// 

	std::list<calls> process_queue;

		// a call could become to-expand.  behavior.

		// we can expand try-all or we can expand add-all

	auto extend_uses = [&]()
	{
		calls::use_space space = calls::get_use_space(uses.size());
		//uses.emplace_back(space.begin(), space.end());
		uses.emplace_back();
		for (auto use : space) {
			uses.back().emplace_back(use);
		}
		std::cout << "Extended uses with " << uses.back().size() << std::endl;
		//uses.back().insert(uses.back().begin(), space.begin(), space.end());
	};

	// each chunk of calls is based off a model call
	// we can store that model call in a different queue,
	// and keep it active
	
	// as things get longer, each model call gets a larger
	// set of stuff added on to it
	// 	but each set is made of subsets, with something
	// 	held constant

	// this appends a whole bunch of call objects
	// but there is no need.  a call object is the same as a list
	// of uses.  maybe is better to assign uses?
	auto append_calls = [&](calls const & source/*, use_iterator & iterator*/)
	{
		// source gets extended
		long index = source.function_count;
		auto & uses_selection = uses[index];
		calls next = source;
		// TODO? it would not be hard to prep the execution state
		// and re-use it here.  would significantly speed up.
		next.push();
		for (auto & use : uses_selection) {
			next.set(use, index);
			process_queue.push_back(next);
		}
	};

	// other way would be, append space
	// 	list of iterators
	
	// say we rewrote it each time
	//
	//	it sounds like optimizing this is difficult
	//	simultaneous with goal
	//
	//considering value of queue at all.
	//maybe queue needs to-expand entries.

	extend_uses();
	calls first;
	first.function_count = 0;
	first.value_count = 0;
	first.set_inputs(1, 2);
	append_calls(first);

	while (process_queue.size()) {
		calls next = process_queue.front();
		process_queue.pop_front();

		// this function working requires the object
		// to not have been copied: which violates its
		// property of quick-copy
		// 	applies only to function_call attribute
		double result = next.one_step(next.size() - 1);//next.call(1,2);
		if (std::fabs(result - 3) < 3 / 1024.) {
			std::cout << next.to_string() + "(1,2) = " + std::to_string(result) << std::endl;
		}

		if (next.function_count >= uses.size()) {
			extend_uses();
		}
		append_calls(next);
			
		// depth.
		// after calling, extend?
		// 	wowch.  hrm.
		// want to focus on combine-operations goal
		// maybe we could look for regressions
		// 	could use relevence, explore different branches
		// 	different depths
		// short-goal was: not regenerate base things.
		// maybe after using a call, we can add it to a used queue.
		// dunno.  reallocating all of these seems a little slow.
		// is
	}


	/*while (true) { // iterates with increasing depth

		// uses has a list of options for each depth
		
		// iterate over each option, keeping shallower ones
		// the same
		// this will need a vector of iterators
		
		// remember: we want to keep our options on a queue
		// as we find them
				// keep partially-set structure
				// inside queue to shuffle value forward 
				//
		// it is unsettling that this is so hard to think
		// we should practice designing queue structures
		// for recursive calls more

		structure {
			calls; // partly filled, size indicates depth
			// behavior: add or process
			// small size makes add, possibly discard
			// full size makes process

			// we can unify the outer loop here
		}
	}*/


	// so, first we want to try every option of the first use
	// then we want to try every option of the second use, with every
	// option of the first
	// we want the trying to know of the first option situation, so
	// it doesn't reapply a bunch of things that are the same.
	// 		

	// each calls will produce a bunch of parallel things
	// onto each one, we can add more. 
	// 	the space can actually be shrunk
	// 	because the second choice is indepedent of the first
	//
}

