#include "funcs.hpp"

#include "../level-1/sugar.hpp"
#include "ref.hpp"


namespace intellect {
using namespace level1;
namespace level2 {

ref context()
{
	static thread_local auto ctx = a("context");
	return ctx;
}

}
}
