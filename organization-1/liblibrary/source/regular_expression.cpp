/*
#define PCRE2_CODE_UNIT_WIDTH sizeof(char)
#include <pcre2.h>

#include <library/regular_expression.hpp>
#include <library/string.hpp>

#include <library/heapvector_definition.cpp>
#include <library/stackvector_definition.cpp>

namespace library {

struct data
{
	pcre2_code * re;
	pcre2_match_data * match;
};

#define re ((data*)this->storage)->re
#define match ((data*)this->storage)->match

regular_expression::regular_expression(string const & expression)
: storage(new data())
{
	int errornumber;
	PCRE2_SIZE erroroffset;
	pcre2_code _re = pcre2_compile(expression.c_str(), PCRE2_ZERO_TERINATED, 0, &errornumber, &erroroffset, NULL);
	if (_re == 0) {
		char buffer[1024];
		pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
		throw std::runtime_error(buffer + " at offset " + std::to_string(erroroffset));
	}
	storage = new data();
	re = _re;
	//match = pcre2_match_data_create_from_pattern(re, NULL);
	match = pcre2_match_data_create(10, NULL);
}

regular_expression::~regular_expression()
{
	pcre2_match_data_free(match);
	pcre2_code_free(re);
}

stackvector<range, 10> regular_expression::find_one(string const & subject, unsigned long offset) const
{
	int rc = pcre2_match(re, subject.c_str(), subject.size(), offset, 0, match, NULL);

	if (rc < 0) {
		return {};
	}
	if (rc == 0) {
		throw std::runtime_error("too many subexpressions; easy to expand but is your pattern maintainable?");
	}

	PCRE_SIZE * ovector = pcre2_get_ovector_pointer(match);
	stackvector<range, 10> result(rc); // crashes if too big
	for (unsigned long i = 0; i < rc; ++ i) {
		result[i] = {ovector[i*2], ovector[i*2+1]};
	}
	return result;
}

} // namespace library
*/
