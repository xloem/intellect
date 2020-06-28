#pragma once

#include "syntax_operator_virtual_unified_by_value.hpp"
#include "syntax_operator_identifier.hpp"

namespace expressions {

#define __opN(name, symbol) template <typename Type> template<typename... Types> \
Type syntax_operator_virtual_unified_by_value<Type>::operator symbol (Types... params) \
{ \
	return syntax_operate(syntax_operator_identifier::name, {params...}); \
}
#define __op3(name, symbol) \
template <typename Type> \
Type syntax_operator_virtual_unified_by_value<Type>::operator symbol (Type param) \
{ \
	return this->syntax_operate(syntax_operator_identifier::name, {param}); \
}
#define __op2(name, symbol) \
template <typename Type> \
Type syntax_operator_virtual_unified_by_value<Type>::operator symbol () \
{ \
	return this->syntax_operate(syntax_operator_identifier::name, {}); \
}
#define __op2int(name, symbol) \
template <typename Type> \
Type syntax_operator_virtual_unified_by_value<Type>::operator symbol (int) \
{ \
	return this->syntax_operator(syntax_operator_identifier::name, {}); \
}

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
//__op(three_way, <=>) // TODO: this is simply for implementing 3 comparators at once
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

}
