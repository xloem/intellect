#include "learning-parts.hpp"

using namespace habitstarts;
using namespace intellect::level1;

static int __init = ([]()->int{

	ahabit(happened-habit,
	{
		ref ev = ctx.get(happens);
		// TODO: perform each associated event
	});

	ahabit(whenever-habit,
	{
	});

	return 0;
})();
