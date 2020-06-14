
// user can provide one class to be handled as expressions.  [is this similar to boost::hana? maybe could augment it]

// let us assume we are using numbers

// so we'll want a class for every value type.

// expressions are made of other expressions, connected with operators

// let's use static functions to pass this over, unless we can reference the predefined operators as functions
// okay ... let's try using global operators for now

#include <any>
#include <functional>
#include <vector>

template <typename T>
class operable
{
public:
	virtual size_t size() = 0;
	virtual std::vector<std::type_info const *> types() = 0;
	virtual void solve(std::vector<T> & arguments, size_t index) = 0;

	struct index_out_of_range : public std::exception { };
	struct indeterminate : public std::exception { };
};

template <typename A, typename B = A, typename C = B>
class operable_any_3 : public operable<std::any>
{
public:
	using func_a = std::function<A(B, C)>;
	using func_b = std::function<B(C, A)>;
	using func_c = std::function<C(A, B)>;
	operable_any_3(func_a forward_a_given_b_c, func_b reverse_b_given_c_a, func_c reverse_c_given_a_b)
	: forward_a_given_b_c(forward_a_given_b_c), reverse_b_given_c_a(reverse_b_given_c_a), reverse_c_given_a_b(reverse_c_given_a_b)
	{ }
	size_t size() override { return 3; }
	std::vector<std::type_info const *> types() override { return {{&typeid(A), &typeid(B), &typeid(C)}}; }
	void solve(std::vector<std::any> & arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<A>(&arguments[0]) = forward_a_given_b_c(*std::any_cast<B>(&arguments[1]), *std::any_cast<C>(&arguments[2]));
			break;
		case 1:
			*std::any_cast<B>(&arguments[1]) = forward_a_given_b_c(*std::any_cast<C>(&arguments[2]), *std::any_cast<A>(&arguments[0]));
			break;
		case 2:
			*std::any_cast<C>(&arguments[2]) = forward_a_given_b_c(*std::any_cast<A>(&arguments[0]), *std::any_cast<B>(&arguments[1]));
			break;
		default:
			throw operable::index_out_of_range();
		}
	}
	func_a forward_a_given_b_c;
       	func_b reverse_b_given_c_a;
       	func_c reverse_c_given_a_b;
};

template <typename A, typename B = A>
class operable_any_2 : public operable<std::any>
{
public:
	using func_a = std::function<A(B)>;
	using func_b = std::function<B(A)>;
	operable_any_2(char const * name, func_a forward_a_given_b, func_b reverse_b_a)
	: forward_a_given_b(forward_a_given_b), reverse_b_given_a(reverse_b_given_a)
	{ }
	size_t size() override { return 2; }
	std::vector<std::type_info const *> types() override { return {{&typeid(A), &typeid(B)}}; }
	void solve(std::vector<std::any> & arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<A>(&arguments[0]) = forward_a_given_b(*std::any_cast<B>(&arguments[1]));
			break;
		case 1:
			*std::any_cast<B>(&arguments[1]) = forward_a_given_b(*std::any_cast<A>(&arguments[0]));
			break;
		default:
			throw operable::index_out_of_range();
		}
	}
	func_a forward_a_given_b;
	func_b reverse_b_given_a;
};

namespace operators
{
#define __f3(functor) \
	template <typename T> \
	std::function<T(T const &, T const &)> func_##functor = std::functor<T>{};
#define __f3_reverse(functor) \
	template <typename T> \
	std::function<T(T const &, T const &)> func_##functor##_reverse = [](T const &a, T const& b) -> T { return func_##functor<T>(b, a); };
#define __b3(functor) \
	template <typename T> \
	std::function<bool(T const &, T const &)> func_##functor = std::functor<T>{};

	__f3(plus) __f3(minus) __f3(negate)
	__f3_reverse(minus)
	__f3(multiplies) __f3(divides)
	__f3_reverse(divides)

	__b3(equal_to) __b3(not_equal_to)
	__b3(greater) __b3(less)
	__b3(greater_equal) __b3(less_equal)

	// equal-to can't be solved for inequal, produces a range
	// let's leave it out for now?
	// 	no this is great.
	// anyway, for the sitation where we explore discerning values,
	// solving a less than for a value would give a range.
	// we would need a type that represents a range, which is quite
	// fine.
	// what about inequal?  this produces a set of anything-but.
	// 	what might you want to do with such a set?
	// 		-> query if soemthing is in it
	// 		-> work with how to change that
	// 		-> combine it with other sets
	// 		-> create something that is within it
	// 		these sets are defined by expressions,
	// 		and nested use of them could get complicated

	__f3(logical_and) __f3(logical_or) __f3(logical_not)

	__f3(bit_and) __f3(bit_or) __f3(bit_xor) __f3(bit_not)

#undef __f3
#undef __b3
	// a = b + c
	template <typename T>
	operable_any_3<T> plus_any(func_plus<T>, func_minus_reverse<T>, func_minus<T>);
	// a = b - c
	template <typename T>
	operable_any_3<T> minus_any(func_minus<T>, func_plus<T>, func_minus_reverse<T>);
	// a = -b
	template <typename T>
	operable_any_2<T> negate_any(func_negate<T>, func_negate<T>);
	// a = b * c
	template <typename T>
	operable_any_3<T> multiplies_any(func_multiplies<T>, func_divides_reverse<T>, func_divides<T>);
	// a = b / c
	template <typename T>
	operable_any_3<T> divides_any(func_divides<T>, func_multiplies<T>, func_divides_reverse<T>);
	// a = b == c
	template <typename T>
	operable_any_3<bool, T> equal_to_any(func_equal_to<T>, [](T const &c, bool const & a) -> T {
		if (a) { return c; }
		else { throw operable<std::any>::indeterminate(); }
	}, [](bool const & a, T const & b) -> T {
		if (a) { return b; }
		else { throw operable<std::any>::indeterminate(); }
	});
};

// for implementing more of this, we'll need a way to pass everything to a particular set of operator classes / handlers / whatever
// some way to proxy or somesuch.  make a group of operators that wrap others.

template <typename T>
class expression
{
public:
	expression<T> operator+(expression<T> const & other) {
		//return T(
	}
};
