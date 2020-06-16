#include <expressions/expressions.hpp>
#include <expressinos/syntax_operator.hpp>

syntax_operator_details const operators[syntax_operators_count] =
{
	{plus, infix, "+", "plus"},
	{minus, infix, "-", "minus"},
	{negate, prefix, "-", "minus"},
	{multiplies, infix, "*", "multiplies"},
	{divides, infix, "/", "divides"},
	{modulus, infix, "%", "modulus"},
	{bit_xor, infix, "^", "bit_xor"},
	{bit_and, infix, "&", "bit_and"},
	{bit_or, infix, "|", "bit_or"},
	{bit_not, prefix, "~", "bit_not"},
	{logical_not, prefix, "!", "logical_not"},
	{assign, infix, "=", "assign"},
	{less, infix, "<", "less"},
	{greater, infix, ">", "greater"},
	{assign_plus, infix, "+=", "assign_plus"},
	{assign_minus, infix, "-=", "assign_minus"},
	{assign_multiplies, infix, "*=", "assign_multiplies"},
	{assign_divides, infix, "/=", "assign_divides"},
	{assign_modulus, infix, "%=", "assign_modulus"},
	{assign_bit_xor, infix, "^=", "assign_bit_xor"},
	{assign_bit_and, infix, "&=", "assign_bit_and"},
	{assign_bit_or, infix, "|=", "assign_bit_or"},
	{shift_left, infix, "<<", "shift_left"},
	{shift_right, infix, ">>", "shift_right"},
	{assign_shift_left, infix, "<<=", "assign_shift_left"},
	{assign_shift_right, infix, ">>=", "assign_shift_right"},
	{equal_to, infix, "==", "equal_to"},
	{not_equal_to, infix, "!=", "not_equal_to"},
	{less_equal, infix, "<=", "less_equal"},
	{greater_equal, infix, ">=", "greater_equal"},
	{logical_and, infix, "&&", "logical_and"},
	{logical_or, infix, "||", "logical_or"},
	{preincrement, prefix, "--", "preincrement"},
	{predecrement, prefix, "--", "predecrement"},
	{postincrement, postfix, "++", "postincrement"},
	{postdecrement, postfix, "++", "postdecrement"},
	{comma, infix, ",", "comma"},
	{star, prefix, "*", "star"},
	{arrow_star, postfix, "->*", "arrow_star"},
	{arrow, postfix, "->", "arrow"},
	{index, bracketing, "[]", "index"},
	{call, bracketing, "()", "call"},
	{three_way, infix, "<=>", "three_way"},
	{cast, prefix, "(cast)", "cast"},
	{literal, postfix, "<literal>", "literal"},
	{syntax_operators_count, ~0, "", "syntax_operators_count"},
};

#define __opN(name, symbol) \
template <typename Class, typename Self, typename... Args> \
class op<syntax_operator_enum::name, Class, Self, Args...> \
{ \
public: \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(args...); \
	} \
}; \
template <typename Class, typename Self> \
class op<syntax_operator_enum::name, Class, Self> \
{ \
public: \
	template <typename... Args> \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(args...); \
	} \
};
#define __op3(name, symbol) \
template <typename Class, typename Self, typename Right> \
class op<syntax_operator_enum::name, Class, Self, Right> \
{ \
public: \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(right); \
	} \
}; \
template <typename Class, typename Self> \
class op<syntax_operator_enum::name, Class, Self> \
{ \
public: \
	template <typename Right> \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(right); \
	} \
};
#define __op2(name, symbol) \
template <typename Class, typename Self> \
class op<syntax_operator_enum::name, Class, Self> \
{ \
public: \
	auto operator symbol () \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(); \
	} \
};
#define __op2int(name, symbol) \
template <typename Class, typename Self> \
class op<syntax_operator_enum::name, Class, Self> \
{ \
public: \
	auto operator symbol (int) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_enum::name>(); \
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
