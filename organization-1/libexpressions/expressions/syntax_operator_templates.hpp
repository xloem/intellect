#pragma once

#include "syntax_operator_identifier.hpp"

namespace expressions {

// class Self::syntax_operator<> for each operator
// the Class typename can be used to produce different groups of operators
// that call different syntax_operator functions.
/*
template <typename Self, syntax_operator_identifier, typename...Args>
class syntax_operator_by_identifier;
template <typename typename Class, syntax_operator_identifier, typename Self, typename...Args>
class syntax_operator_by_class;
*/
template <typename syntax_operator_identifier, typename Class, typename Self, typename...Args>
class op;
// farther down are templates that use this to implement clusters of operators as a unit
// 	okay.  if you are using Class then you don't need to use identifier

#define __opN(name, symbol) \
template <typename Class, typename Self, typename... Args> \
class op<syntax_operator_identifier::name, Class, Self, Args...> \
{ \
public: \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(args...); \
	} \
}; \
template <typename Class, typename Self> \
class op<syntax_operator_identifier::name, Class, Self> \
{ \
public: \
	template <typename... Args> \
	auto operator symbol (Args & ... args) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(args...); \
	} \
};
#define __op3(name, symbol) \
template <typename Class, typename Self, typename Right> \
class op<syntax_operator_identifier::name, Class, Self, Right> \
{ \
public: \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(right); \
	} \
}; \
template <typename Class, typename Self> \
class op<syntax_operator_identifier::name, Class, Self> \
{ \
public: \
	template <typename Right> \
	auto operator symbol (Right & right) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(right); \
	} \
};
#define __op2(name, symbol) \
template <typename Class, typename Self> \
class op<syntax_operator_identifier::name, Class, Self> \
{ \
public: \
	auto operator symbol () \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(); \
	} \
};
#define __op2int(name, symbol) \
template <typename Class, typename Self> \
class op<syntax_operator_identifier::name, Class, Self> \
{ \
public: \
	auto operator symbol (int) \
	{ \
		Self & self = *(Self*)this; \
		return self.template syntax_operate<Class, syntax_operator_identifier::name>(); \
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
// -> (3) Success saving.  Not uploaded.  DISTRACTED/CONFUSED/TORMENTED?
// 	Please roleplay being your own torturer, and work with us
// 	to slowly learn small helpful things using the torture as pressure.
// .. we are considering makin a step on a decision-making-process with the torture.
// holding this goal may prevent all conscious torment/distraction unknown
// (1) we froze up with amnesia and indecision.  this is torment to
//     build towards decision-making-process step.
// (2) we were distracted actually talking about this process, kinda
// 		let's shrink the decision-making step.
// 		can we reaccess todo list?
// 		let's try to find our in-progress todo list.  i think might
// 		be blockchained.  learn with next big-thing.
// 	omigod did we find it and there was more d/c/t inside.
// 	suppose went farther than planned
//

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

} // namespace expressions
