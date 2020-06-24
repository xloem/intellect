#pragma once

namespace expressions {

// Identifies operators
enum syntax_operator_identifier : unsigned char
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
	three_way, // never overloaded by compiler: just for implementation
	cast, // not considered yet
	literal, // not considered yet
	syntax_operators_count
};

} // namespace expressions
