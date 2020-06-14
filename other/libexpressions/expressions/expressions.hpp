
// user can provide one class to be handled as expressions.  [is this similar to boost::hana? maybe could augment it]

// let us assume we are using numbers

// so we'll want a class for every value type.

// expressions are made of other expressions, connected with operators

// let's use static functions to pass this over, unless we can reference the predefined operators as functions
// okay ... let's try using global operators for now

#include <any>
#include <functional>
#include <vector>

class operable
{
public:
	virtual size_t size() = 0;
	virtual std::vector<std::type_info*> types() = 0;
	virtual void solve(std::vector<std::any> arguments, size_t index) = 0;

	struct index_out_of_range : public std::exception { };
};

template <typename T>
class operable_3
{
public:
	using func = std::function<T(T, T)>;
	operable_3(func forward_a_given_b_c)
	: forward_a_given_b_c(forward_a_given_b_c)
	{ }
	operable_3(func forward_a_given_b_c, func reverse_b_given_c_a, func reverse_c_given_a_b)
	: forward_a_given_b_c(forward_a_given_b_c), reverse_b_given_c_a(reverse_b_given_c_a), reverse_c_given_a_b(reverse_c_given_a_b)
	{ }
	size_t size() override { return 3; }
	std::vector<std::type_info*> types() override { return std::vector<std::type_info*>{&typeid(T), &typeid(T), &typeid(T)}; }
	void solve(std::vector<std::any> arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<T>(&arguments[0]) = forward_a_given_b_c(*std::any_cast<T>(&arguments[1]), *std::any_cast<T>(&arguments[2]));
			break;
		case 1:
			*std::any_cast<T>(&arguments[1]) = forward_a_given_b_c(*std::any_cast<T>(&arguments[2]), *std::any_cast<T>(&arguments[0]));
			break;
		case 2:
			*std::any_cast<T>(&arguments[2]) = forward_a_given_b_c(*std::any_cast<T>(&arguments[0]), *std::any_cast<T>(&arguments[1]));
			break;
		default:
			throw operable::index_out_of_range();
		}
	}
	func forward_a_given_b_c, reverse_b_given_c_a, reverse_c_given_a_b;
};

template <typename T>
class operable_2
{
public:
	using func = std::function<T(T)>;
	operable_2(char const * name, func forward_a_given_b, func reverse_b_a)
	: forward_a_given_b(forward_a_given_b), reverse_b_given_a(reverse_b_given_a)
	{ }
	size_t size() override { return 2; }
	void solve(std::vector<std::any> arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<T>(&arguments[0]) = forward_a_given_b(*std::any_cast<T>(&arguments[1]));
			break;
		case 1:
			*std::any_cast<T>(&arguments[1]) = forward_a_given_b(*std::any_cast<T>(&arguments[0]));
			break;
		default:
			throw operable::index_out_of_range();
		}
	}
	func forward_a_given_b, reverse_b_given_a;
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

	__f3(logical_and) __f3(logical_or) __f3(logical_not)

	__f3(bit_and) __f3(bit_or) __f3(bit_xor) __f3(bit_not)

#undef __f3
#undef __b3
	template <typename T>
	operable_3<T> plus(func_plus<T>, func_minus_reverse<T>, func_minus<T>);
};

template <typename T>
class expression
{
public:
	expression<T> operator+(expression<T> const & other) {
		//return T(
	}
};
