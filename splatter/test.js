#!/usr/bin/env node

let evaluate = require('./eval.js')

//eval('console.log("hello")');

ideas = [
	"console",
	"log",
	".",
	'"',
	"hello",
	"(",
	")"
]

// tried 'console'.  success.  evaluated to description of console object.  bad-behavior, process-needs-development
result = messy_eval('console log hello')
console.log(result)

function mess_with(previous, change, offset)
{
	let idea = previous.slice(0, previous.search(change) + offset)
	while (Math.random() > 0.1) {
		idea += global.ideas[Math.floor(Math.random() * global.ideas.length)];
	}
	return idea;
}

function messy_eval(idea)
{
	do {
		try {
			console.log("? ... " + idea)
			return evaluate(idea)
		} catch (e) {
			if (e.problem) {
				idea = mess_with(idea, e.problem, e.problemoffset)
			} else {
				console.log(e)
				throw null
			}
		}
	} while(true);
}

function rational_eval(idea)
{

}

// too much of this
successes = [
	"A(B)",
	"A.B(C)",
	"A;",
	"'A'",
];
