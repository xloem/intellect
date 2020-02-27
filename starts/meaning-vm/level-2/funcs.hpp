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
ref newnotepad(ref name, bool imaginechanges = false);
ref subnotepad(ref name, bool allowouter = false, bool allowself = false);
ref noteconcept(std::any data = {});
void checknotepad(ref concept);
bool innotepad(ref concept, ref pad);
void leavenotepad(ref concept, ref pad);
//void considerwith(ref realitypad, ref imaginationpad);
ref imagineget(ref pad, ref concept, bool change = false, bool allowoutofcontext = false);
ref imagineset(ref pad, ref concept, bool allowoutofcontext = false); // this does not presently update concepts that use the one passed.
void entersubnotepad(ref concept, ref name, bool allowouter = false, bool allowself = false);
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
