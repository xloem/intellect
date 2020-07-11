#include <expressions/syntax_operator.hpp>
#include <expressions/syntax_operator_identifier.hpp>
#include <expressions/syntax_operator_fixing.hpp>

namespace expressions {

extern syntax_operator const operators_storage[syntax_operators_count];

syntax_operator const * operators = operators_storage;

syntax_operator const operators_storage[syntax_operators_count] =
{
	{literal,            postfix,         0,  0, "<literal>", "literal"},
	{scope,              infix,           1,  0, "::",        "scope"},
	{postincrement,      postfix,         2,  0, "++",        "postincrement"},
	{postdecrement,      postfix,         2,  0, "--",        "postdecrement"},
	{call,               postsurroundfix, 2,  0, "()",        "call"},
	{index,              postsurroundfix, 2,  0, "[]",        "index"},
	{dot,                infix,           2,  0, ".",         "dot"},
	{arrow,              infix,           2,  0, "->",        "arrow"},
	{preincrement,       prefix,          3,  1, "++",        "preincrement"},
	{predecrement,       prefix,          3,  1, "--",        "predecrement"},
	{posate,             prefix,          3,  1, "+",         "posate"},
	{negate,             prefix,          3,  1, "-",         "minus"},
	{logical_not,        prefix,          3,  1, "!",         "logical_not"},
	{bit_not,            prefix,          3,  1, "~",         "bit_not"},
	{cast,               prefix,          3,  1, "(cast)",    "cast"},
	{star,               prefix,          3,  1, "*",         "star"},
	{address,            prefix,          3,  1, "&",         "address"},
	{dot_star,           infix,           4,  0, ".*",        "dot_star"},
	{arrow_star,         infix,           4,  0, "->*",       "arrow_star"},
	{multiplies,         infix,           5,  0, "*",         "multiplies"},
	{divides,            infix,           5,  0, "/",         "divides"},
	{modulus,            infix,           5,  0, "%",         "modulus"},
	{plus,               infix,           6,  0, "+",         "plus"},
	{minus,              infix,           6,  0, "-",         "minus"},
	{shift_left,         infix,           7,  0, "<<",        "shift_left"},
	{shift_right,        infix,           7,  0, ">>",        "shift_right"},
	{three_way,          infix,           8,  0, "<=>",       "three_way"},
	{less,               infix,           9,  0, "<",         "less"},
	{less_equal,         infix,           9,  0, "<=",        "less_equal"},
	{greater,            infix,           9,  0, ">",         "greater"},
	{greater_equal,      infix,           9,  0, ">=",        "greater_equal"},
	{equal_to,           infix,           10, 0, "==",        "equal_to"},
	{not_equal_to,       infix,           10, 0, "!=",        "not_equal_to"},
	{bit_and,            infix,           11, 0, "&",         "bit_and"},
	{bit_xor,            infix,           12, 0, "^",         "bit_xor"},
	{bit_or,             infix,           13, 0, "|",         "bit_or"},
	{logical_and,        infix,           14, 0, "&&",        "logical_and"},
	{logical_or,         infix,           15, 0, "||",        "logical_or"},
	{assign,             infix,           16, 1, "=",         "assign"},
	{assign_plus,        infix,           16, 1, "+=",        "assign_plus"},
	{assign_minus,       infix,           16, 1, "-=",        "assign_minus"},
	{assign_multiplies,  infix,           16, 1, "*=",        "assign_multiplies"},
	{assign_divides,     infix,           16, 1, "/=",        "assign_divides"},
	{assign_modulus,     infix,           16, 1, "%=",        "assign_modulus"},
	{assign_shift_left,  infix,           16, 1, "<<=",       "assign_shift_left"},
	{assign_shift_right, infix,           16, 1, ">>=",       "assign_shift_right"},
	{assign_bit_and,     infix,           16, 1, "&=",        "assign_bit_and"},
	{assign_bit_xor,     infix,           16, 1, "^=",        "assign_bit_xor"},
	{assign_bit_or,      infix,           16, 1, "|=",        "assign_bit_or"},
	{comma,              infix,           17, 0, ",",         "comma"},
};

}
