#include "bootstrap-perser.hpp"

namespace intellect {
namespace level3 {

void parsebootstrap(ref stream, ref context)
{
	istream & ss = *stream.val<istream*>();
	while (true) {
		string cmd;
		ss >> cmd;
		if (!ss) { break; }
		if (cmd == "//") {
			// could parse comments into file info
			i implmented comments.  they are missing.b
		} else if (cmd == "information") {
			// change to information habitname [
			//   needs ...
			//   makes ...
			// ]
			ref args = makeconcept();
			string name;
			ss >> name;
			string linerest;
			std::getline(ss, linerest);
			stringstream ss2(linerest);
			while (true) {
				string arg;
				ss2 >> arg;
				if (!ss2) { break; }
				args.link("information-order", arg);
			}
			ref("set-steps")(name, args);
		} else if (cmd == "when") {
			string name;
			ss >> name;
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-when"); }
			std::map<string,ref> labels;
			std::set<string> values;
			values.insert("context");
			values.insert("self");
			ref order = makehabitinformationorder(name);
			for (ref arg: order.getAll("information-order")) {
				values.insert(arg.name());
			}
			conceptunmake(order);
			// need to seed values with argument names
			ref laststep = name;
			labels["return"] = nothing;
			// when dump group [
			// 	= is-in-set in-set group
			// 	? is-in-set if true return.
			// 		period-at-end: goto.
			// 		comma-or-colon-at-end: label
			// 	output-name group
			// 	output-name ":" // quotes mean always-global
			// ]
			//
			// proposing expression-based now.
			// haven't resolved inherited name-contexts with literal strings fully.
			// 	we'll need a function that turns a symbol into a ref, and takes
			// 	an inherited context.
			// 	we'll also change write-name to output-text, and get the name attribute
			// 		what opens an inherited context? when are symbols added to it?
			// 		atm we have a list of steps has 1 context.
			// 			we also have labels to refer to.
			// 			put labels in the context, treat them as normal names.
			// 				that sounds nice, for vm to be able to pass step references to functions
			// 				would just be a literal, though, a constant
			//			or we could not do subblocks, expression as steps
			//		what if we were to write this using the steps, with a local context
			//		we would have to track labels, and put them in the surrounding local context.  maybe also a local condition.
			//		let's make a context object, link labels and surrounding condition to it.
			// working on conditions.
			// 	propose if tracks last step
			// 	when if ends, adds last step to condition's set of last steps
			// 	then next step after condition can wire to all steps in the set.
			// 	can use 1-element set for normal steps.
			// 	change step-creation to not automatically wire.
			// 	and reconsider condition-step to not use its next-step attribute.
			// 	instead its conditions decide what the next step is.
			// 		looks good for conditions.  fix names and update whole thing.
			// inside a [], each step enters a set, to be wired to the next step inbetween.
			// for jump-labels, we'll need to pass a reference to names of them to the
			// function that builds the [] step list.
			// 	this reference to names is basically a name-context.  a lookup map for names.  name-context is a concept related to the name link that inherits via outer-context links.
			// 		it shows what to get for the name link
			// 	to move towards name-contexts, let's at least call it name-context.
			// 	maybe make a function to do the lookup.
			// 		label-name-context.
			// 	it's roughly okay to branch anywhere within the funtion, so it doesn't ned to actually inherit.
			while (true) {
				string label, action, result;
				ss >> action;
				if (action == "]") { break; }
				if (action[action.size()-1] == ':' || action[action.size()-1] == ',') {
					label = action;
					label.resize(label.size() - 1);
					if (label == "return") { throw makeconcept().link(is, "return-label-used"); }
					ss >> action;
				}
				if (action == "=" || action == "set") {
					ss >> result;
					ss >> action;
					values.insert(result);
				}
				if (action[action.size()-1] == '.') {
					// is goto
					action.resize(action.size() - 1);
					if (!labels.count(action)) {
						labels.emplace(action, makeconcept());
					}
					labels[action].link("label", action);
					if (laststep.linked("next-step")) { throw makeconcept().link(is, "jump-from-nowhere", "label", action); }
					laststep.link("next-step", labels[action]);
					laststep = nothing;
					continue;
				}
				if (action == "if") {
					ref cond = lookup(parsevalue(stream));
					ss >> action;
					if (action[action.size()-1] != '.') {
						throw makeconcept().link(is, "condition-is-not-label", "action", action, "cond", cond);
					}
					if (!laststep.isa("condition-step")) {
						throw makeconcept().link(is, "if-not-following-condition", "cond", cond, "action", action);
					}
					if (label.size()) {
						throw makeconcept().link(is, "if-case-has-label", "cond", cond, "action", action, "label", label);
					}
					action.resize(action.size()-1);
					if (!labels.count(action)) {
						labels.emplace(action, makeconcept());
						labels[action].link("label", action);
					}
					ref("condition-step-set")(laststep, cond, labels[action]);
					// if this improves from being  jump, remember to
					// update laststep to end of any 'anything' branch
					continue;
				}
				if (laststep == nothing && label.size() == 0) { throw makeconcept().link(is, "no-path-to-code"); }
				if (label.size() && !labels.count(label)) {
					labels[label] = makeconcept();
					labels[label].link("label", label);
				}
				ref nextstep = label.size() ? labels[label] : makeconcept();
				if (action == "?" || action == "pick") {
					string cond;
					ss >> cond;
					if (!values.count(cond)) {
						throw makeconcept().link(is, "condition-must-be-in-context", condition, cond);
					}
					laststep = ref("set-condition-step")(nextstep, laststep, cond, makeconcept().link("anything", "nothing"));
				} else {
					// otherwise, action is an action, and we have to read the right number of args
					if (laststep.isa("condition-step")) {
						if (ref("condition-step-get")(laststep, "anything") != "nothing") {
							if (label.size() == 0) {
								throw makeconcept().link(is, "condition-already-has-anything-branch-and-steps-follow", condition, laststep);
							}
						} else {
							ref("condition-step-set")(laststep, "anything", nextstep);
						}
					} else if (laststep != nothing) {
						laststep.link("next-step", nextstep);
					}
					ref habit = values.count(action) ? action : lookup(action);
					ref order = makehabitinformationorder(habit);
					ref neededmap = makeconcept();
					ref knownmap = makeconcept();
					string linerest;
				       	std::getline(ss, linerest);
					stringstream ss2(linerest);
					ref stream2 = alloc(intellect::level0::concepts::allocations(), (istream*)&ss2);
					for (ref arg : order.getAll("information-order")) {
						ref argname = parsevalue(stream2);
						if (!ss2) { break; }
						// depending on whether argname is in localcontext, pass to neededmap or knownmap.  also parse literal strings.
						if (values.count(argname.name())) {
							neededmap.link(arg, argname);
						} else {
							knownmap.link(arg, lookup(argname));
						}
					}
					conceptunmake(order);
					dealloc(stream2, intellect::level0::concepts::allocations());
					ref mademap = makeconcept();
					if (result.size()) {
						mademap.link("result", values.count(result) ? result : lookup(result));
					}
					ref("set-context-step")(nextstep, "nothing", knownmap, neededmap, mademap, habit);
					laststep = nextstep;
				}
			} 
		} else {
			throw ref("parse-error").link("stream", stream, "unexpected-word", cmd);
		}
	}
}

}
}
