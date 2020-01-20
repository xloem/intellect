#pragma once

#include "common.hpp"

#include <functional>
#include <initializer_list>

namespace intellect {
namespace level2 {

ref & context();
ref notepad();
void checknotepad(ref concept);
void enternotepad(ref concept);
void leavenotepad(ref concept);
ref noteconcept();
ref makehabit(ref name, std::initializer_list<ref> infonames, std::function<void(ref)> code);
ref makehabitinformationorder(ref habit);
void habitassume(ref habit, ref information, ref value);
ref dohabit(ref habit, std::initializer_list<ref> args = {});
ref dohabit(ref habit, std::initializer_list<std::initializer_list<ref>> args);

void rethrowref();

}
}
