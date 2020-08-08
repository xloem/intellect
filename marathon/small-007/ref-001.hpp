#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

template <class t>
using il = std::initializer_list<t>;

// holds concept data, rarely used directly
struct concept;

// refers to concept data, can be copied lightweight
class ref {
};

// holds concept data; rarely used directly
// we were thinking link objects would be eased if iterators uniquely differentiated
// links better
//
// note: std::multimap does not invalidate iterators on mutation
// std::unordered_multimap does
