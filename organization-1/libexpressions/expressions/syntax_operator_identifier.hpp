#pragma once

namespace expressions {

// Identifies operators
enum syntax_operator_identifier : unsigned char
{
	literal, // not considered yet
	scope,
	postincrement,
	postdecrement,
	call,
	index,
	dot,
	arrow,
	preincrement,
	predecrement,
	posate, // unary plus
	negate,
	logical_not,
	bit_not,
	cast, // not considered yet
	star,
	address,
	dot_star,
	arrow_star,
	multiplies,
	divides,
	modulus,
	plus,
	minus,
	shift_left,
	shift_right,
	three_way, // never overloaded by compiler: just for implementation
	less,
	less_equal,
	greater,
	greater_equal,
	equal_to,
	not_equal_to,
	bit_and,
	bit_xor,
	bit_or,
	logical_and,
	logical_or,
	assign,
	assign_plus,
	assign_minus,
	assign_multiplies,
	assign_divides,
	assign_shift_left,
	assign_shift_right,
	assign_modulus,
	assign_bit_and,
	assign_bit_xor,
	assign_bit_or,
	comma,
	syntax_operators_count
};

} // namespace expressions
