
// seems reasons are patterns that pick things that produce what the reason is for.  pattern has history of working. [that's after associative-nearness]

// so you'll need some judgers; a basic judger could be associative-nearness

// non-immediate-relevence, do-all
// reasons-for-reasons: time-nearness etc


// contextual information: contextual environment

main(){
	easy environment = "environment";

	easy self = "self";
	environment[self] = self;

	easy call-environment;
	call-environment["event"] = "word-from-user";
	call-environment["word"] = "hello";
	call-environment["time"] = (any)(now);
	call-environment["environment"] = environment;

	// contextuality seems a little confused, if it matters
	// 	[all-1-context is way to go here]
	// 		reasons ...

	self["handle-event"] = [](easy call-environment) {
		self["note-interesting"](call-environment);
		if (call-environment["merit"] != null()) {

		}
		if (good || bad) {
			// build direct relevence
		}
		if (relevence indicates can make good or bad) {
			// possibly build indirect relevence
		}
	};

	// so 1 form of immediate relevence would be happens-within-time-interval
	// non-immediate would then be, happens-within-time-interval of something that happens-within-time-interval.

	self["select-behavior"] = [](easy call-environment) {
		
	};

	self["last-interesting"] = null();

	self["note-interesting"] = [](easy call-environment) {
		// build relevence, store log
		easy event;
		event["event"] = call-environment;
		event["time"] = (any)(now);
		event["preceding"] = self["last-interesting"];
		self["last-interesting"] = event;
	};

	easy say-word = "say-word";
	say-word = [](easy call-environment) {
		string::line(call-environment.data<string>());
	};

	self["perform-behavior"] = [](easy call-environment) {
		self["note-interesting"](call-environment);	
	};

	self["call-all"] = [](easy call-environment) {
		for (index_t i = 0; i < call-environment.order-count().data<index_t>(); ; ++ i) {
			call-environment.order-get(i)(call-environment);
		}
	};
	
	easy relevence["relevence"];
	relevence["for"] = "things";
	

	// relevence would need a kind: we value time-preceding relevence here
	// be nice to be able to hook on relevence: don't have event-hooks yet
	// but can make a bag of them and call all

	// relevence is a comparable degree
	// many relevences are complex things: not numbers.
	// we need a way to decide what is most relevent.
		// could something put it in an ordered list?
	// then, relevence has reasons
	

	// want things that produce good in future
	

	environment["input"] = input;
	// get-stuff a bunch of words from user, including sometimes 'good' or 'bad'
	// do-stuff has tools to modify graph.  relate to user when used, at first
	// the core addition is global timing association with 'good' and 'bad'
	// we'll want a wayness for finding contextual relevence with 'good' or 'bad'
		// liekly have 1 do-stuff for guessing good things [provides reason-good or reason-bad]
		// another do-stuff for getting amount [provides reason-better or reason-worse]
}
