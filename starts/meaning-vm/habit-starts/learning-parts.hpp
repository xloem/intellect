#pragma once

#include "common.hpp"

namespace habitstarts {

// first need ability to trigger on stuff.
// whenever A happens, do B.
// stop doing B whenever A happens.
// when A happens, do B once.

decl(action, happens, context);
decl(happened); // happened-habit(ctx) performs actions associated with ctx[happens]
decl(whenever); // whenever-habit(ctx) stores to do ctx[action] when ctx[happens] happens
                // providing ctx[action-context]
decl(stop); decl(when); // stop-when-habit(ctx) removes doing ctx[happens] on ctx[event]
decl(once); // once-habit(ctx) stores to do ctx[action] the next time ctx[happens] happens
            // providing ctx[action-context]

/*
	Testing metric: runs when event is fired, measures time between
	  event and right time.  if [usual] time is less than ever before, success.
	  if time is significantly more than behavior's norm, failure.
	Convert to English: try to have the event happen at the right time.
*/
// starting out by making a judgement habit that occasionally provides 'good' or 'bad' to things, to lead how to develop
	// for fairness, seems reasonable to provide a pattern showing reason for good or bad
// 

// set to do 1ce at goal time:
// 	ctx X
// 	set to do 1ce on goal time:
// 		set next-happened (local)
// 	delay (a sequence of habits that do nothing)
// 	if next-happened is set
// 		remove something from delay
// 	otherwise
// 		add something to delay (wait for unspecified user-perceptible time, selected from discrete set)
// 	provide adjusted delay to next context

}
