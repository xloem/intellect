#include "bootstrap-perser.hpp"

namespace intellect {
namespace level3 {

ref parsevalue(ref stream)
{
	istream & ss = *stream.val<istream*>();
	string word;
	ss >> word;
	if (word.size() > 0 && (word[0] == '"' || word[0] == '\'' || word[0] == '`')) {
		char delim = word[0];
		string accum = word;
		if (accum[accum.size()-1] != delim || accum.size() == 1) {
			char c;
			while ((c = ss.get()) != delim) {
				accum += c;
			} 
			accum += c;
		} else {
			//accum.resize(accum.size() - 1);
		}
		word = accum;
	}
	return word;
}

void parsebootstrap(ref stream, ref context)
{
	istream & ss = *stream.val<istream*>();
	string lookupstr;
	ss >> lookupstr;
	ref lookup = lookupstr;
	std::list<std::string> comments;
	while (true) {
		string cmd;
		ss >> cmd;
		if (!ss) { break; }
		if (cmd == "//") {
			std::string comment;
			std::getline(ss, comment);
			comments.push_back(comment);
		} else if (cmd == "concept") {
			string name;
			ss >> name;
			ref c = lookup(parsevalue(name));
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-concept"); }
			while (true) {
				string type, target;
				ss >> type;
				if (type == "]") { break; }
				ss >> target;
				c.link(lookup(parsevalue(type)), lookup(parsevalue(target)));
			}
		} else if (cmd == "habit") {
			string name;
			ss >> name;
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-name"); }
			ref args = makeconcept();
			while (true) {
				string arg;
				ss >> arg;
				if (ss == "]") { break; }
				args.link("information-order", arg);
			}
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-needs"); }
			std::set<string> values;
			values.insert("context");
			values.insert("self");
			while (true) {
				string arg;
				ss >> arg;
				if (ss == "]") { break; }
				args.link("information-made", arg);
				// need to seed values with argument names
				values.insert(arg.name());
			} // information-made is unused.  is a 'stub' for if-needed-later.
			ref("set-steps")(name, args);
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-makes"); }
			std::map<string,ref> labels;
			ref laststep = name;
			labels["return"] = nothing;
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			while (true) {
				string label, action, result;
				ss >> action;
				if (action == "]") { break; }
				if (action == "//") {
				}
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
				// if (action == "concept") {
				// }
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
					ref informationnames = makeconcept();
					string linerest;
				       	std::getline(ss, linerest);
					stringstream ss2(linerest);
					ref stream2 = alloc(intellect::level0::concepts::allocations(), (istream*)&ss2);
					auto args = order.getAll("information-order");
					auto argsit = args.begin();
					while (true) {
						ref argname = parsevalue(stream2);
						if (!ss2) { break; }
						// depending on whether argname is in localcontext, pass to neededmap or knownmap.  also parse literal strings.
						informationnames.link("name", argname);
						if (argsit != args.end()) {
							if (values.count(argname.name())) {
								neededmap.link(*argsit, argname);
							} else {
								knownmap.link(*argsit, lookup(argname));
							}
							++ argsit;
						}
					}
					knownmap.link("information-names", informationnames);
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
