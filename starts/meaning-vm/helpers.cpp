#include "helpers.hpp"

#include "meaning.hpp"
#include "memorystore.hpp"

#include <unordered_map>
#include <functional>

// sometimes we use an assignment as an expression.
// is there any reason not to support this?
// 	well, we would evaluate the assignment when it is converted to a ref
// 	and currently that is used for something else
// what is it used for?
// 	I don't remember ... the assignment is destroyed and unevaluated, so it's likely
// 	throughout the internal implemntation
// 		it's used for the comma operator, which doesn't want evaluation.
// okay, hmm.
// 	well, if we wanted to support this, we could change the comma operator to not do the
// 	conversion.  use some other trick.
// 	[ ] change comma operator to not do conversion: instead take statementcallref object
// 	[ ] change statementcallref::operator ref() to evaluate and return the statement
// since we're accumulating more possible bugs, let's think about design.
// really, ref should be different from syntax sugar.  we should have a special syntax sugar
// object, and it should not use its own functions at all.  that means not using meaning functions
// that use them eiher.
// 	work also brings to light how the product will be made in layers, with each layer using more interdependence and automatic meaning, etc, than the lower one.
// 	layer0 is raw references and allocations
//	layer1 is very core meaning
// 	layer2 is very core syntax sugar
// 		each layer can only use layers below it.

// concept names are for bootstrapping convenience,
// to make hardcoding structures easier.
// hence there is just one single list of them
std::unordered_map<std::string,concept*,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;

struct name_t : public ref
{
	name_t();
} name;

static ref statement_function("statement-function");
static ref statement_evaluated("statement-evaluated");
static ref true_ref("true");
static ref false_ref("false");

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

statementcallref ref::operator=(ref that)
{
	// for single-line evaluation, we'll need to return a special object
	declrefs(assign, expression);
	decllnks(value, to);
	statementcallref ret(assign-expression, [](ref expr){
		declrefs(link, target, unknown, source, type);
		decllnks(value, to);
		lnks(link-target, link-type, link-source);
		decllnks(name, is, anonymous);
		ref lhs = expr->get(to);
		ref rhs = expr->get(value);
		if (lhs.isa(link) && lhs->get(link-target) == unknown) {
			// we are a link missing a target: our assignment is making the link happen
			lhs->unlink(link-target, unknown);
			lhs->link(link-target, rhs);
			ref src = lhs->get(link-source);
			if (lhs->get(link-type) != unknown && src != unknown) {
				src->link(lhs->get(link-type), rhs);
				expr->unlink(to, lhs);
				dealloc(lhs);
			} else {
				throw std::logic_error("not sure what to do with incomplete link assignment");
			}
		} else if (rhs->linked(anonymous, true) && !lhs->linked(anonymous, true)) {
			// this is assignment of anonymous content to empty named concept
			bool donealready = false;
			for (auto & l : lhs->links) {
				if (ref(l.first) == name) { continue; }
				if (ref(l.first) == is && ref(l.second) == link-type) { continue; }
				donealready = true;
			}
			if (donealready) {
				// if we have links, and that has links we do not have, an error has been made
				for (auto & link : rhs->links) {
					if (ref(link.first) == anonymous) { continue; }
					if (ref(link.first) == name) { continue; }
					if (!lhs->linked(ref(link.first), ref(link.second))) {
						throw std::logic_error(lhs.name() + " already defined otherwise from " + rhs->get(is).name());
					}
				}
				donealready = true;
			}
			rhs->unlink(anonymous, true);
			auto nam = rhs->get(name);
			rhs->unlink(name, nam);
			if (!donealready) {
				lhs->links.insert(rhs->links.begin(), rhs->links.end());
			}
			rhs->link(name, nam);
			expr->unlink(value, rhs);
			dealloc(rhs);
			dealloc(nam);
		} else {
			throw std::logic_error("unexpected bare assignment");
		}
	});
	ret.r->link(to, *this);
	ret.r->link(value, that);
	return ret;
/*
	decllnks(anonymous, is, name);
	declrefs(link, source, type, target, unknown);
	lnks(link-target, link-source, link-target);
	if (this->isa(link) && ptr->get(link-target) == unknown) {
	} else if (that->linked(anonymous, true) && !ptr->linked(anonymous, true)) {
		// TODO TODO: when left hand side is a link, and right hand side is anonymous,
		// 	especially when left hand side is link without other content,
		// 	there is no way to determine if user is providing content for link,
		//	or is making a link on another ref[a = b].
		//	instead, return just assignment information, and process in outer context.
		//	will want to make a ref subclass and assign in destructor unless has been used elsewhere.

	} else if (isa(link-type)) {
		// assignment to a link-type is likely inside a [type1=target1,type2=target2] expression
		// this happens after checking for name assignment because when name assignment
		// 	is redundant, the left-hand value could be a link
		return (*this) << that;
	}
	throw std::logic_error("unexpected use of assignment");
*/
}

/*ref ref::operator<<(ref target)
{
	// prep a link
	ref ret = alloc();
	ret->link(*this, target);
	return ret;
}*/

void statementcallref::destatement()
{
	auto func = r->get(statement_function);
	r->unlink(statement_function);
	dealloc(func);
}

statementcallref::statementcallref(ref type, std::function<void(ref)> func)
: r(a(type).ptr)
{
	r->link(statement_function, valloc<std::function<void(ref)>>(func));
}

statementcallref::statementcallref(ref const & that)
: r(that.ptr)
{
	if (!that->linked(statement_function)) {
		throw std::logic_error(std::string(that) + " has no statement-function");
	}
}

statementcallref::operator ref()
{
	ref ret(r);
	destatement();
	r.ptr = 0;
	return ret;
}

#include <iostream>
statementcallref::~statementcallref() noexcept
{
	if (r.ptr == 0) { return; }
	if (r->linked(statement_evaluated, true_ref)) {
		std::cerr << "statement already evaluated: " << r << std::endl;
		return;
	}
	try {
		auto func = r->vget<std::function<void(ref)>>(statement_function, true);
		func->data(r);
		destatement();
		r->link(statement_evaluated, true_ref);
		dealloc(r);
		r.ptr = 0;
	} catch (std::out_of_range &) { }
}

statementcallref::statementcallref(statementcallref && that) noexcept
: r(that.r.ptr)
{
	that.r.ptr = 0;
}

statementcallref::statementcallref(statementcallref & that)
: r(that.r.ptr)
{
	if (that.r.ptr == 0) { throw std::logic_error("empty ref"); }
	if (that.r->linked(statement_function)) {
		// statements are moved, for evaluation
		that.r.ptr = 0;	
	}
}

ref ref::operator[](ref expr) {
	declrefs(assign, comma, expression);
	decllnks(value, to, what);
	declrefs(link, type, unknown);

	if (expr.isa(assign-expression)) {
		ref lhs = expr->get(to);
		ref rhs = expr->get(value);
		ptr->link(lhs, rhs);
		dealloc(expr);
		return *this;
	} else if (expr.isa(comma-expression)) {
		auto parts = expr->getAll(what);
		dealloc(expr);
		for (ref part : parts) {
			if (!part.isa(assign-expression)) {
				throw std::logic_error("[,] argument is not an assignment");
			}
			operator[](part);
		}
		return *this;
	} else if (expr.isa(link-type)) {
		return ::link(*this, expr, unknown);
	} else {
		throw std::logic_error("[] argument is neither a link nor an assignment");
	}
}

statementcallref operator,(statementcallref a, statementcallref b)
{
	declrefs(comma, expression);
	decllnks(what);
	if (a.r.isa(comma-expression)) {
		if (b.r.isa(comma-expression)) {
			a.r->links.insert(b.r->links.begin(), b.r->links.end());
			dealloc(b); b.r.ptr = 0;
		} else {
			a.r->link(what, b.r); b.r.ptr = 0;
		}
		return a;
	} else if (b.r.isa(comma-expression)) {
		b.r->link(what, a.r); a.r.ptr = 0;
		return b;
	} else {
		statementcallref ret(comma-expression, [](ref) { throw std::logic_error("bare comma-expression"); });
		ret.r->link(what, a.r); a.r.ptr = 0;
		ret.r->link(what, b.r); b.r.ptr = 0;
		return ret;
	}
}

template <>
vref<std::string>::vref(std::string const & s)
: ptr(valloc(s).ptr)
{
	ptr->link(::name, ptr);
}
name_t::name_t()
: ref(alloc().ptr)
{
	vref nam(std::string("name"));
	ptr->link(::name, nam);
	conceptsByName.emplace(nam, ptr);
}

ref::ref(std::string const & s)
{
	auto res = conceptsByName.find(s);
	if (res != conceptsByName.end()) {
		ptr = res->second;
	} else {
		ref con = alloc();
		vref<std::string> nam(s);
		conceptsByName.emplace(nam, con.ptr);
		con->link(::name, nam);
		ptr = con.ptr;
	}
}

vref<std::string> ref::name() const
{
	try {
		return ptr->vget<std::string>(::name, true);
	} catch (std::out_of_range &) {
		declrefs(UNNAMED);
		return UNNAMED.name();
	}
}

ref::operator const char *() const {
	return name()->data.c_str();
}

ref a(ref what)
{
	static unsigned long long gid = 0;
	decllnks(is, anonymous);
	ref ret(what.name() + "-" + std::to_string(gid++));
	ret->link(is, what);
	ret->link(anonymous, true);
	return ret;
}
ref a(ref what, ref name)
{
	if (!name.isa(what)) {
		decllnks(is);
		name[is = what];
	}
	return name;
}
ref an(ref what)
{
	return a(what);
}
ref an(ref what, ref name)
{
	return a(what, name);
}

bool ref::isa(ref what) const
{
	declrefs(is);
	for (auto group : ptr->getAll(is)) {
		if (group == what) return true;
		if (group == *this) continue;
		if (group.isa(what)) return true;
	}
	return false;
}

bool ref::isan(ref what) const
{
	return isa(what);
}
