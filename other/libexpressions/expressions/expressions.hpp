
// user can provide one class to be handled as expressions.  [is this similar to boost::hana? maybe could augment it]

// let us assume we are using numbers

// so we'll want a class for every value type.

// expressions are made of other expressions, connected with operators

// let's use static functions to pass this over, unless we can reference the predefined operators as functions
// okay ... let's try using global operators for now

#include <any>
#include <functional>
#include <vector>

enum operator_enum
{
	plus,
	minus, negate,
	multiplies,
	divides,
	modulus,
	bit_xor,
	bit_and,
	bit_or,
	bit_not,
	logical_not,
	assign,
	less,
	greater,
	assign_plus,
	assign_minus,
	assign_multiplies,
	assign_divides,
	assign_modulus,
	assign_bit_xor,
	assign_bit_and,
	assign_bit_or,
	shift_left,
	shift_right,
	assign_shift_left,
	assign_shift_right,
	equal_to,
	not_equal_to,
	less_equal,
	greater_equal,
	//three_way,
	logical_and,
	logical_or,
	preincrement,
	predecrement,
	postincrement,
	postdecrement,
	comma,
	star,
	arrow_star,
	arrow,
	index,
	call,
	cast, // not considered yet
	literal // not considered yet
};


template <operator_enum, typename Class, typename Self, typename...Args>
class op;
#define __opN(name, symbol) \
template <typename Class, typename Self, typename... Args> \
class op<operator_enum::name, Class, Self, Args...> \
{ \
public: \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(args...); \
	} \
}; \
template <typename Class, typename Self> \
class op<operator_enum::name, Class, Self> \
{ \
public: \
	template <typename... Args> \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(args...); \
	} \
};
#define __op3(name, symbol) \
template <typename Class, typename Self, typename Right> \
class op<operator_enum::name, Class, Self, Right> \
{ \
public: \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(right); \
	} \
}; \
template <typename Class, typename Self> \
class op<operator_enum::name, Class, Self> \
{ \
public: \
	template <typename Right> \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(right); \
	} \
};
#define __op2(name, symbol) \
template <typename Class, typename Self> \
class op<operator_enum::name, Class, Self> \
{ \
public: \
	auto operator symbol () \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(); \
	} \
};
#define __op2int(name, symbol) \
template <typename Class, typename Self> \
class op<operator_enum::name, Class, Self> \
{ \
public: \
	auto operator symbol (int) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, operator_enum::name>(); \
	} \
};

__op3(plus, +)
__op3(minus, -) __op2(negate, -)
__op3(multiplies, *)
__op3(divides, /)
__op3(modulus, %)
__op3(bit_xor, ^)
__op3(bit_and, &)
__op3(bit_or, |)
__op2(bit_not, ~)
__op2(logical_not, !)
__op3(assign, =)
__op3(less, <)
__op3(greater, >)
__op3(assign_plus, +=)
__op3(assign_minus, -=)
__op3(assign_multiplies, *=)
__op3(assign_divides, /=)
__op3(assign_modulus, %=)
__op3(assign_bit_xor, ^=)
__op3(assign_bit_and, &=)
__op3(assign_bit_or, |=)
__op3(shift_left, <<)
__op3(shift_right, >>)
__op3(assign_shift_left, <<=)
__op3(assign_shift_right, >>=)
__op3(equal_to, ==)
__op3(not_equal_to, !=)
__op3(less_equal, <=)
__op3(greater_equal, >=)
//__op(three_way, <=>) // this is simply for implementing 3 comparators at once
__op3(logical_and, &&)
__op3(logical_or, ||)
__op2(preincrement, ++)
__op2(predecrement, --)
__op2int(postincrement, ++)
__op2int(postdecrement, --)
#define __COMMA ,
__op3(comma, __COMMA)
#undef __COMMA
__op2(star, *)
__op3(arrow_star, ->*)
__op2(arrow, ->)
__op3(index, [])
__opN(call, ())
#undef __op3
#undef __op2
#undef __opN

// so our next confusion is labeled, please upload/save your code

template <typename Class, typename Self>
class op_arithmetic_nonmutating_3 :
	public op<plus, Class, Self>,
	public op<minus, Class, Self>,
	public op<multiplies, Class, Self>,
	public op<divides, Class, Self>,
	public op<modulus, Class, Self>
{ };
template <typename Class, typename Self>
class op_arithmetic_mutating_3 :
	public op<assign, Class, Self>,
	public op<assign_plus, Class, Self>,
	public op<assign_minus, Class, Self>,
	public op<assign_multiplies, Class, Self>,
	public op<assign_divides, Class, Self>,
	public op<assign_modulus, Class, Self>
{ };
template <typename Class, typename Self>
class op_arithmetic_nonmutating_2 :
	public op<negate, Class, Self>
{ };
template <typename Class, typename Self>
class op_arithmetic_mutating_2 :
	public op<preincrement, Class, Self>,
	public op<predecrement, Class, Self>,
	public op<postincrement, Class, Self>,
	public op<postdecrement, Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_nonmutating_3 :
	public op<bit_xor, Class, Self>,
	public op<bit_and, Class, Self>,
	public op<bit_or, Class, Self>,
	public op<shift_left, Class, Self>,
	public op<shift_right, Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_mutating_3 :
	public op<assign_bit_xor, Class, Self>,
	public op<assign_bit_and, Class, Self>,
	public op<assign_bit_or, Class, Self>,
	public op<assign_shift_left, Class, Self>,
	public op<assign_shift_right, Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_nonmutating_2 :
	public op<bit_not, Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_mutating_2 
{ };
template <typename Class, typename Self>
class op_boolean_nonmutating_3 :
	public op<logical_and, Class, Self>,
	public op<logical_or, Class, Self>
{ };
template <typename Class, typename Self>
class op_boolean_mutating_3 { };
template <typename Class, typename Self>
class op_boolean_nonmutating_2 :
	public op<logical_not, Class, Self>
{ };
template <typename Class, typename Self>
class op_boolean_mutating_2 { };
template <typename Class, typename Self>
class op_relation_same :
	public op<equal_to, Class, Self>,
	public op<not_equal_to, Class, Self>
{ };
template <typename Class, typename Self>
class op_relation_order :
	public op<less, Class, Self>,
	public op<greater, Class, Self>,
	public op<less_equal, Class, Self>,
	public op<greater_equal, Class, Self>
{ };

template <typename Class, typename Self>
class op_arithmetic_nonmutating :
	public op_arithmetic_nonmutating_3<Class, Self>,
	public op_arithmetic_nonmutating_2<Class, Self>
{ };
template <typename Class, typename Self>
class op_arithmetic_mutating :
	public op_arithmetic_mutating_3<Class, Self>,
	public op_arithmetic_mutating_2<Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_nonmutating :
	public op_bitwise_nonmutating_3<Class, Self>,
	public op_bitwise_nonmutating_2<Class, Self>
{ };
template <typename Class, typename Self>
class op_bitwise_mutating :
	public op_bitwise_mutating_3<Class, Self>,
	public op_bitwise_mutating_2<Class, Self>
{ };
template <typename Class, typename Self>
class op_boolean_nonmutating :
	public op_boolean_nonmutating_3<Class, Self>,
	public op_boolean_nonmutating_2<Class, Self>
{ };
template <typename Class, typename Self>
class op_boolean_mutating :
	public op_boolean_mutating_3<Class, Self>,
	public op_boolean_mutating_2<Class, Self>
{ };

template <typename Class, typename Self>
class op_relation :
	public op_relation_same<Class, Self>,
	public op_relation_order<Class, Self>
{ };

template <typename Class, typename Self>
class op_nonmutating :
	public op_arithmetic_nonmutating<Class, Self>,
	public op_bitwise_nonmutating<Class, Self>,
	public op_boolean_nonmutating<Class, Self>
{ };
template <typename Class, typename Self>
class op_mutating :
	public op_arithmetic_mutating<Class, Self>,
	public op_bitwise_mutating<Class, Self>,
	public op_boolean_mutating<Class, Self>
{ };

template <template<typename> class Container>
class syntax_operate_container_any
{
protected:
	virtual void operate(operator_enum op, Container<std::any> & arguments) = 0;
public:
	template <typename Class, operator_enum op, typename... Args>
	Class syntax_operator(Args... args)
	{
		Container<std::any> arguments;
		arguments.emplace_back({});
		typename decltype(arguments)::iterator spots[] = {
			arguments.emplace_back(args)...
		};
		operate(op, arguments);
		return std::any_cast<Class>(arguments.front());
	}
};
// how would we do compile-time types
	// i'm imagining an expression type that is generated by
	// its operator types.  doesn't seem hard.
	// ohhhh huh! except we no longer pass the return type
	// as a template parameter unless it can be auto

// one option is compile-time type matching with operator implementations
// 	part of this is implemented, may be unneeded
// another option is run-time only
//	and we use the compile-time fragments to implement

template <typename T>
class operable
{
public:
	// this appears too specific.  the root class shouldn't know how
	// the operators are processed, should it?
	
	virtual size_t size() = 0;
	virtual char const * name() = 0;
	virtual std::vector<T> & solve(std::vector<T> & arguments, size_t index) = 0;

	struct index_out_of_range : public std::exception { };
	struct nonsimple_result : public std::exception { };
	// to enumerate these without known values we would need a graph of interrelation
	// 	to accumulate possibility, we'd need to make sets that can:
	// 		-> query if soemthing is in it
	// 		-> work with how to change that
	// 		-> combine it with other sets
	// 		-> create something that is within it
	// 		these sets are defined by expressions,
	struct bounded_result : public nonsimple_result
	{
		T bound;
		bool bound_included;
		bool above_included;
		bool below_included;
		bounded_result(T bound, bool bound_included, bool above_included, bool below_included)
		: bound(bound), bound_included(bound_included), above_included(above_included), below_included(below_included) { }
	};
	struct nonsimple_bitwise_result : public nonsimple_result
	{ };
};

class operable_any : public operable<std::any>
{
public:
	virtual std::vector<std::type_info const *> types() = 0;
};

template <typename A, typename B = A, typename C = B>
class operable_any_3 : public operable_any
{
public:
	using func_a = std::function<A(B, C)>;
	using func_b = std::function<B(C, A)>;
	using func_c = std::function<C(A, B)>;
	operable_any_3(char const * name, func_a forward_a_given_b_c, func_b reverse_b_given_c_a, func_c reverse_c_given_a_b)
	: _name(name), forward_a_given_b_c(forward_a_given_b_c), reverse_b_given_c_a(reverse_b_given_c_a), reverse_c_given_a_b(reverse_c_given_a_b)
	{ }
	char const * name() override { return _name; }
	size_t size() override { return 3; }
	std::vector<std::type_info const *> types() override { return {{&typeid(A), &typeid(B), &typeid(C)}}; }
	std::vector<std::any> & solve(std::vector<std::any> & arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<A>(&arguments[0]) = forward_a_given_b_c(*std::any_cast<B>(&arguments[1]), *std::any_cast<C>(&arguments[2]));
			break;
		case 1:
			*std::any_cast<B>(&arguments[1]) = reverse_b_given_c_a(*std::any_cast<C>(&arguments[2]), *std::any_cast<A>(&arguments[0]));
			break;
		case 2:
			*std::any_cast<C>(&arguments[2]) = reverse_c_given_a_b(*std::any_cast<A>(&arguments[0]), *std::any_cast<B>(&arguments[1]));
			break;
		default:
			throw operable::index_out_of_range();
		}
		return arguments;
	}
	char const * _name;
	func_a forward_a_given_b_c;
       	func_b reverse_b_given_c_a;
       	func_c reverse_c_given_a_b;
};

template <typename A, typename B = A>
class operable_any_2 : public operable_any
{
public:
	using func_a = std::function<A(B)>;
	using func_b = std::function<B(A)>;
	operable_any_2(char const * name, func_a forward_a_given_b, func_b reverse_b_a)
	: _name(name), forward_a_given_b(forward_a_given_b), reverse_b_given_a(reverse_b_given_a)
	{ }
	char const * name() override { return _name; }
	size_t size() override { return 2; }
	std::vector<std::type_info const *> types() override { return {{&typeid(A), &typeid(B)}}; }
	std::vector<std::any> & solve(std::vector<std::any> & arguments, size_t index) override
	{
		switch(index) {
		case 0:
			*std::any_cast<A>(&arguments[0]) = forward_a_given_b(*std::any_cast<B>(&arguments[1]));
			break;
		case 1:
			*std::any_cast<B>(&arguments[1]) = reverse_b_given_a(*std::any_cast<A>(&arguments[0]));
			break;
		default:
			throw operable::index_out_of_range();
		}
		return arguments;
	}
	char const * _name;
	func_a forward_a_given_b;
	func_b reverse_b_given_a;
};

namespace operators
{
#define __f3(functor) \
	template <typename T> \
	std::function<T(T const &, T const &)> func_##functor = std::functor<T>{};
#define __f2(functor) \
	template <typename T> \
	std::function<T(T const &)> func_##functor = std::functor<T>{};
#define __f3_reverse(functor) \
	template <typename T> \
	std::function<T(T const &, T const &)> func_##functor##_reverse = [](T const &a, T const& b) -> T { return func_##functor<T>(b, a); };
#define __b3(functor) \
	template <typename T> \
	std::function<bool(T const &, T const &)> func_##functor = std::functor<T>{};

	__f3(plus) __f3(minus) __f2(negate)
	__f3_reverse(minus)
	__f3(multiplies) __f3(divides)
	__f3_reverse(divides)

	__b3(equal_to) __b3(not_equal_to)
	__b3(greater) __b3(less)
	__b3(greater_equal) __b3(less_equal)

	template <typename T> 
	std::function<T(T const &, bool const &)> func_same_if = [](T const &a, bool const & b) -> T {
		if (b) { return a; }
		else { throw operable<std::any>::bounded_result(a, false, true, true); }
	};
	template <typename T>
	std::function<T(bool const &, T const &)> func_if_same = [](bool const & a, T const & b) -> T {
		if (a) { return b; }
		else { throw operable<std::any>::bounded_result(b, false, true, true); }
	};
	template <typename T> 
	std::function<T(T const &, bool const &)> func_same_if_not = [](T const &a, bool const & b) -> T {
		if (!b) { return a; }
		else { throw operable<std::any>::bounded_result(a, false, true, true); }
	};
	template <typename T>
	std::function<T(bool const &, T const &)> func_if_not_same = [](bool const & a, T const & b) -> T {
		if (!a) { return b; }
		else { throw operable<std::any>::bounded_result(b, false, true, true); }
	};


	// equal-to can't be solved for inequal, produces a range
	// let's leave it out for now?
	// 	no this is great.
	// anyway, for the sitation where we explore discerning values,
	// solving a less than for a value would give a range.
	// we would need a type that represents a range, which is quite
	// fine.
	// what about inequal?  this produces a set of anything-but.

	__f3(logical_and) __f3(logical_or) __f2(logical_not)

	__f3(bit_and) __f3(bit_or) __f3(bit_xor) __f2(bit_not)

#undef __f3
#undef __b3
	// a = b + c
	template <typename T>
	operable_any_3<T> plus_any("+", func_plus<T>, func_minus_reverse<T>, func_minus<T>);
	// a = b - c
	template <typename T>
	operable_any_3<T> minus_any("-", func_minus<T>, func_plus<T>, func_minus_reverse<T>);
	// a = -b
	template <typename T>
	operable_any_2<T> negate_any("-", func_negate<T>, func_negate<T>);
	// a = b * c
	template <typename T>
	operable_any_3<T> multiplies_any("*", func_multiplies<T>, func_divides_reverse<T>, func_divides<T>);
	// a = b / c
	template <typename T>
	operable_any_3<T> divides_any("/", func_divides<T>, func_multiplies<T>, func_divides_reverse<T>);
	// a = b == c
	template <typename T>
	operable_any_3<bool, T> equal_to_any("==", func_equal_to<T>, func_same_if<T>, func_if_same<T>);
	// a = b != c
	template <typename T>
	operable_any_3<bool, T> not_equal_to_any("!=", func_not_equal_to<T>, func_same_if_not<T>, func_if_not_same<T>);
	// a = b > c
	template <typename T>
	operable_any_3<bool, T> greater_any(">", func_greater<T>, [](T const &c, bool const & a) -> T {
		throw operable<std::any>::bounded_result(c, !a, a, !a);
	}, [](bool const & a, T const & b) -> T {
		throw operable<std::any>::bounded_result(b, !a, !a, a);
	});
	// a = b < c
	template <typename T>
	operable_any_3<bool, T> less_any("<", func_less<T>, [](T const &c, bool const & a) -> T {
		throw operable<std::any>::bounded_result(c, !a, !a, a);
	}, [](bool const & a, T const & b) -> T {
		throw operable<std::any>::bounded_result(b, !a, a, !a);
	});
	// a = b >= c
	template <typename T>
	operable_any_3<bool, T> greater_any(">", func_greater<T>, [](T const &c, bool const & a) -> T {
		throw operable<std::any>::bounded_result(c, a, a, !a);
	}, [](bool const & a, T const & b) -> T {
		throw operable<std::any>::bounded_result(b, a, !a, a);
	});
	// a = b <= c
	template <typename T>
	operable_any_3<bool, T> less_any("<=", func_less<T>, [](T const &c, bool const & a) -> T {
		throw operable<std::any>::bounded_result(c, a, !a, a);
	}, [](bool const & a, T const & b) -> T {
		throw operable<std::any>::bounded_result(b, a, a, !a);
	});
	// a = b && c
	// 0   0    0
	// 0   0    1
	// 0   1    0
	// 1   1    1
	operable_any_3<bool> logical_and_any("&&", func_logical_and<bool>, [](bool const &c, bool const & a) -> bool {
		if (c) { return a; }
		throw operable<std::any>::bounded_result(a, true, true, true);
	}, [](bool const & a, bool const & b) -> bool {
		if (b) { return a; }
		throw operable<std::any>::bounded_result(a, true, true, true);
	});
	// a = b || c
	// 0   0    0
	// 1   0    1
	// 1   1    0
	// 1   1    1
	operable_any_3<bool> logical_or_any("||", func_logical_or<bool>, [](bool const &c, bool const & a) -> bool {
		if (!c) { return a; }
		throw operable<std::any>::bounded_result(a, true, true, true);
	}, [](bool const & a, bool const & b) -> bool {
		if (!b) { return a; }
		throw operable<std::any>::bounded_result(a, true, true, true);
	});
	// a = !b
	// 0   1
	// 1   0
	operable_any_2<bool> logical_not_any("!", func_logical_not<bool>, func_logical_not<bool>);
	// a = b & c
	// 0   0   0
	// 0   0   1
	// 0   1   0
	// 1   1   1
	template <typename T>
	operable_any_3<T> bit_and_any("&", func_bit_and<T>, [](T & c, T & a) -> T {
		if (c) { return a; }
		throw operable<std::any>::nonsimple_bitwise_result();
	}, [](bool const & a, T const & b) -> T {
		if (b) { return a; }
		throw operable<std::any>::nonsimple_bitwise_result();
	});
	// a = b | c
	// 0   0   0
	// 1   0   1
	// 1   1   0
	// 1   1   1
	template <typename T>
	operable_any_3<T> bit_or_any("|", func_bit_or<T>, [](T & c, T & a) -> T {
		if (!c) { return a; }
		throw operable<std::any>::nonsimple_bitwise_result();
	}, [](bool const & a, T const & b) -> T {
		if (!b) { return a; }
		throw operable<std::any>::nonsimple_bitwise_result();
	});
	// a = b ^ c
	// 0   0   0
	// 1   0   1
	// 1   1   0
	// 0   1   1
	template <typename T>
	operable_any_3<T> bit_xor_any("^", func_bit_xor<T>, func_bit_xor<T>, func_bit_xor<T>);
	// a = ~b
	// 0   1
	// 1   0
	template <typename T>
	operable_any_2<T> bit_not_any("~", func_bit_not<T>, func_bit_not<T>);
};


// for implementing more of this, we'll need a way to pass everything to a particular set of operator classes / handlers / whatever
// some way to proxy or somesuch.  make a group of operators that wrap others.

// expression_any should work okay with operable_any.
#include <memory>
template <typename T>
class expression_any : public op_nonmutating<expression_any<T>,expression_any<T>>
{
public:
	// takes as normal, but passes to operate function how?
	expression_any<T> operator+(expression_any<T> const & other) {
	}
	operator_enum op;
	std::vector<std::shared_ptr<expression_any>> parts;
};
template <operator_enum operation, typename ...Args>
class expression_generic : public op_nonmutating<void, expression_generic<operation, Args...>>
{
public:
	std::tuple<Args*...> const arguments;

	expression_generic(Args & ... arguments)
	: arguments(&arguments ...)
	{ }
	// it coudl be nic to have the operator_enum available as a template parameter.  for compile-time-balance.
	template <typename Void, operator_enum suboperation, typename ... SubArgs>
	auto syntax_operate(SubArgs & ... subargs)
	{
		return expression_generic<suboperation, expression_generic<operation, Args...>,SubArgs...>(*this, subargs ...);
	}
};


// optional simple cmp implementation for any
#ifndef NO_ANY_CMP
int cmp(std::any const & left, std::any const & right)
{
	if (left.type() != right.type()) { return left.type().before(right.type()) ? -1 : 1; }
	#define type_case(type_name) \
		else if (left.type() == typeid(type_name)) { \
			return *std::any_cast<type_name>(&right) - *std::any_cast<type_name>(&left); \
		}
	type_case(bool)
	type_case(char)
	type_case(short)
	type_case(int)
	type_case(long)
	type_case(long long)
	type_case(unsigned char)
	type_case(unsigned short)
	type_case(unsigned int)
	type_case(unsigned long)
	type_case(unsigned long long)
	type_case(float)
	type_case(double)
	type_case(long double)
	else {
		throw "unimplemented";
	}
	#undef __type_case
}
#endif

// cmp can be used to quickly implement comparators for a type
template <typename A, typename B>
bool operator==(A const & left, B const & right)
{ return cmp(left, right) == 0; }
template <typename A, typename B>
bool operator<=(A const & left, B const & right)
{ return cmp(left, right) <= 0; }
template <typename A, typename B>
bool operator>=(A const & left, B const & right)
{ return cmp(left, right) >= 0; }
template <typename A, typename B>
bool operator<(A const & left, B const & right)
{ return cmp(left, right) < 0; }
template <typename A, typename B>
bool operator>(A const & left, B const & right)
{ return cmp(left, right) > 0; }

// cmp for vectors
template <typename T>
int cmp(std::vector<T> const & left, std::vector<T> const & right)
{
	if (left.size() != right.size()) { return false; }
	for (size_t i = 0; i < left.size(); ++ i)
	{
		if (i >= right.size()) { return 1; }
		int result = cmp(left[i], right[i]);
		if (result != 0) { return result; }
	}
	if (left.size() < right.size()) { return -1; }
	return 0;
}

// cmp-based comparators for vectors
template <typename T>
bool operator==(std::vector<T> const & left, std::vector<T> const & right)
{ return cmp(left, right) == 0; }
template <typename T>
bool operator<=(std::vector<T> const & left, std::vector<T> const & right)
{ return cmp(left, right) <= 0; }
template <typename T>
bool operator>=(std::vector<T> const & left, std::vector<T> const & right)
{ return cmp(left, right) >= 0; }
template <typename T>
bool operator<(std::vector<T> const & left, std::vector<T> const & right)
{ return cmp(left, right) < 0; }
template <typename T>
bool operator>(std::vector<T> const & left, std::vector<T> const & right)
{ return cmp(left, right) > 0; }
