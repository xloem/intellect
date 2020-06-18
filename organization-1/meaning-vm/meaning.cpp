#include "meaning.hpp"

#include "helpers.hpp"

ref avariable(ref name)
{
	declrefs(variable);
	return a(variable, name);
}

ref link(ref sourceref, ref typeref, ref targetref)
{
	declrefs(link, source, type, target);
	lnks(link-source, link-type, link-target);
	return a(link)[
		link-source = sourceref,
		link-type = typeref,
		link-target = targetref
	];
}

ref not(ref whatref)
{
	declrefs(not);
	decllnks(what);
	return a(not, not-whatref)[what] = whatref;
}
