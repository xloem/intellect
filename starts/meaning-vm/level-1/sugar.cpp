#include "sugar.hpp"

#include "concepts.hpp"

using namespace intellect::level1;
using namespace concepts;

namespace intellect {
namespace level1 {

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

ref a(ref group)
{
	static unsigned long long gid = 0;
	ref ret(group.name() + "-" + std::to_string(gid++));
	ret.link(is, group);
	ret.link(is, anonymous);
	return ret;
}
ref a(ref group, ref name)
{
	if (!name.isa(group)) {
		name.link(is, group);
	}
	return name;
}
ref an(ref group)
{
	return a(group);
}
ref an(ref group, ref name)
{
	return a(group, name);
}

}
}
