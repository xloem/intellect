#pragma once
#include "habit.hpp"

namespace sym {
	symbol(from);
	symbol(type);
	symbol(to);
}

namespace act {

cxxhabit set({},{sym::from, sym::type, sym::to},[](ref ctx)
{
	ref from = ctx[sym::from];
	from.set(ctx[sym::type], ctx[sym::to]);
});

}
