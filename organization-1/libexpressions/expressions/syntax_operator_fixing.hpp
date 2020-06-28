#pragma once

namespace expressions {

// Specifies operator placement
enum syntax_operator_fixing : unsigned char {
	prefix,
	infix,
	postfix,
	postsurroundfix,
};

}
