#pragma once

#include "../level-2/ref.hpp"

namespace intellect {
namespace level3 {

//void parsebootstrap(ref stream, ref context);
void loadhabits();
std::string ref2txt(intellect::level2::ref r);
intellect::level2::ref txt2ref(std::string s);

}
}
