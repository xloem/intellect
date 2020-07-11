#pragma once

#include <library/any.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace library {

class multiany : public typable_valued
{
public:
private:
	boost::multi_index_container<
		library::any,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<boost::multi_index::tag<type_info>, boost::multi_index::const_mem_fun<library::any,type_info const&,&any::type>
		>
	> container;
};

} // library
