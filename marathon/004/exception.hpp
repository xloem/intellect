#include "ref.hpp"

namespace sym {
	symbol(misinterpreted_as);
	symbol(instead_of);
}

class exception : public ref, public std::runtime_error
{
public:
	exception(il<std::pair<ref,ref>> refs)
	: ref(refs, std::string()), runtime_error(dump(true))
	{
		ref::data<std::string>() = runtime_error::what();
	}
};
