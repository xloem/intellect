#include "funcs.hp"

#include "../level-1/sugar.hpp"

namespace intellect {
namespace level2 {

level2::ref context()
{
	static thread_local level1::ref ctx = level1::a("context");
	return ctx;
}

}
}
