
// seems reasons are patterns that pick things that produce what the reason is for.  pattern has history of working. [that's after associative-nearness]

// so you'll need some judgers; a basic judger could be associative-nearness

// non-immediate-relevence, do-all
// reasons-for-reasons: time-nearness etc


// contextual information: contextual environment

#include <node4/easy.hpp>
#include <library/time.hpp>

void trigger-event(easy self, easy event, easy data, easy context)
{
	easy call-environment;
	call-environment["event"] = event;
	call-environment["data"] = data;
	call-environment["time"] = (any)(library::seconds_monotonic());
	call-environment["content"] = context;
	self["handle-event"](call-environment);
}

void make-core-behaviors(easy self)
{
	self["call-all"] = [](easy call-environment) {
		for (index_t i = 0; i < call-environment.order-count().data<index_t>(); ; ++ i) {
			call-environment.order-get(i)(call-environment);
		}
	};

	self["note-interesting"] = [self](easy call-environment) {
		// build relevence, store log
		easy event;
		event["event"] = call-environment;
		event["time"] = (any)(library::seconds_monotonic());
		event["preceding"] = self["last-interesting"];
		self["last-interesting"] = event;
	};

	self["learn-around"] = [self](easy call-environment) {
		//trigger-event(self, "learn-around", call-environment, reference::null());
		
		// basic time association relevence?
	 	
		// link this-event to nearby-events
		// will need types-of-events, which is fine [good/bad for now]

	};


}

void make-behaviors(easy self)
{
	make-core-behaviors(self);

	easy say-word = "say-word";
	say-word = [](easy call-environment) {
		string::line(call-environment.data<string>());
	};
	self[say-word] = say-word;
}

void setup-environment(easy environment)
{

	easy self = "self";
	environment[self] = self;
	self[environment] = environment;
	
	make-behaviors(self);
}

void trigger-input(easy environment)
{
	input user("input");
	easy self = environment["self"]
	self["input"] = user;
	while (true) {
		easy word = input.word();
		trigger-event(self, "input-word", word, input);
	}
}

int main()
{
	easy environment = "environment";
	setup-environment(environment);

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

	self["perform-behavior"] = [](easy call-environment) {
		self["note-interesting"](call-environment);	
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
