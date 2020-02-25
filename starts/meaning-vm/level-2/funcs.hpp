#pragma once

#include "common.hpp"
#include "../level-0/concept.hpp"

#include <any>
#include <functional>
#include <initializer_list>

namespace intellect {
namespace level2 {

ref & context();

ref & notepad();
ref newnotepad(ref name);
ref subnotepad(ref name, bool allowouter = false);
ref noteconcept(std::any data = {});
void checknotepad(ref concept);
bool innotepad(ref concept, ref pad);
void leavenotepad(ref concept, ref pad);
void considerwith(ref realitypad, ref imaginationpad);
ref imagineget(ref pad, ref concept);
ref imagineset(ref pad, ref concept); // this will update references _only_ within pad, so relevent concepts must also be brought in
void entersubnotepad(ref concept, ref name, bool allowouter = false);
//void givename(ref context, ref concept, std::string const & name, bool contextisnotepad = false);
//ref namelookup(std::string name, ref context, ref outerlink, bool makelocal, bool contextisnotepad = false);
ref getnamed(std::string name, bool create = false);

//void bootstrap2notepad(std::string name);

ref makehabit(ref name, std::initializer_list<ref> infonames, std::function<void(ref)> code);
ref makehabitinformationorder(ref habit);
void habitassume(ref habit, ref information, ref value);
ref dohabit(ref habit, std::initializer_list<ref> args = std::initializer_list<ref>());
ref dohabit(ref habit, std::initializer_list<std::initializer_list<ref>> args, bool extra_information = false);

void rethrowref();

using vector = std::vector<intellect::level0::concept*>;
using vectorit = vector::iterator;

}
}
