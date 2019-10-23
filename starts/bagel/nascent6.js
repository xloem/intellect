// This file was WIP, two prongs were being pursued at once when work was halted.

// Meaning Representation
class Property {
	constructor(type, dest) {
		this.type = type
		this.dest = dest
	}
}
class Properties {
	// members are all Usage Simplification defining Promise Spec
	constructor(type, from) {
		this.props = from ? from.props.slice() : []
		//if (from) {console.log('INHERIT: '); for (let prop of this.props) { console.log(prop) }}
		this.add('is', type)
	}
	toString() {
		let str = ''
		for (let prop of this.props) {
			str += prop.type + ':' + prop.dest + ' '
		}
		return str
	}
	add(type, dest) {
		this.props.push(new Property(type, dest))
	}
	// recommend hoisting variables when designing them, so they don't match everything
	has(type, dest) {
		for (let p of this.props)
			if ((p.type == type || isVar(type))
			    && (p.dest == dest || isVar(dest)))
				return true
		return false
	}
	del(type, dest) {
		for (let i = 0; i < this.props.length; ++ i) {
			let p = this.props[i]
			if (p.type == type && p.dest == dest) {
				this.props.splice(i, 1)
				return true
			}
		}
		return false
	}
	get(type) {
		let result = null
		for (let i = 0; i < this.props.length; ++ i) {
			let p = this.props[i]
			if (p.type == type) {
				if (result) throw new Error('get on multiply defined property')
				result = p.dest
			}
		}
		return result
	}
	hasIsVar() {
		return this.has('is','variable')
	}
}
class AbstractData {
	constructor(from) {
		this.props = new Properties('abstract', from && from.props)
	}
}
class StringData extends String {
	constructor(str, from) {
		super(str)
		this.props = new Properties('string', from && from.props)
		this.props.del('is', 'array')
	}
}
class ArrayData extends Array {
	constructor(arr, from) {
		super(...arr)
		this.props = new Properties('array', from && from.props)
		this.props.del('is', 'string')
	}
}

// Meaning Representation
class Var extends StringData {
	constructor(name){
		super(name)
		props.add('is','variable')
	}
	// recommend e.g. 'is','variable-rep' when code is designing them, so don't match everything
	// opencog uses a 'quote' wrapper to cause this, sounds more general
}
function isVar(x) {
	return x.props && x.props.has('is','variable')
}
const ONE = StringData('one')
const MANY = StringData('many')
const VAR_X = new Var('X')

// MEMORY
// the archivist.  Provides for needs of data.  Memory is relevent whenever there is
// an informational need.
//
// 	When remembering steps, likely what is relevent is what is needed to reach the goal
// 		Here's a possible step memory.
//
//		let ret = new StringData('labeled ' + items.length + ' text-string')
//		ret.props.add('is','step-memory')
//		ret.props.add('step',genproptextstring)
//		ret.props.add('ram',ram)
//		ret.props.add('count',items.length)
//		for (let a of items)
//			ret.props.add('item', a)

class ActiveMemory extends ArrayData {
	// although Memory is eventually a module that provides for stored information needs
	// this class is just a group of concepts, moved in and out as needed
	constructor(items, from) {
		super(items || [], from)
		this.props.add('is','working-ram')
	}
	clone() {
		return new ActiveMemory(this, this);
	}
	add(...items) {
		//if (this.cloned) { this = this.slice(); this.cloned = false; }
		for (let item of items)
			this.push(item)
	}
	del(...items) {
		//if (this.cloned) { this = this.slice(); this.cloned = false; }
		for (let item of items) {
			let index = this.indexOf(item)
			this.splice(index, 1)
		}
	}
	contains(...items) {
		for (let item of items)
			if (this.indexOf(item) === -1) return false
		return true
	}
	containsWith(...props) {
		let ret = []
		outer: for (let a of this) {
			for (let i = 0; i < props.length; i += 2)
				if (! a.props.has(props[i], props[i+1]))
					continue outer
			return true
		}
		return false
	}
	getWith(...props) {
		let ret = []
		outer: for (let a of this) {
			//console.log(a.props.props)
			for (let i = 0; i < props.length; i += 2) {
				//console.log(props[i] + ': ' + props[i+1] + '?')
				if (! a.props.has(props[i], props[i+1]))
					continue outer
			}
			ret.push(a)
		}
		if (ret.length > 0) return ret
		return null
	}
}
// note: for now we produce only data; state-changes are data-changes
// TODO: this class has methods.  turn them into promise-meeting-habits.  a promise is a spec for behavior, e.g. how data is stored.  this spec can be interpeted by a more general habit.
class FunctionData {
	constructor(name, // name of function
	            func, // function taking ordered needs
	                  // ordered array of made objects is returned
	            needs, // array of 1-property-per-object this function requires
	            makes // array of 1-property-per-object this function produces
	) {
		this.name = name
		this.props = new Properties('function')
		this.props.add('name', name)
		this.call = func
		// If needed, make atomspace-like pattern structures for needs/makes.
		// 	- represent multiple properties on 1 needed object
		// 	- represent input objects present in output
		this.needs = needs
		this.makes = makes
	}
	makesFrom(ram) { // what we make with the ram
		// match needs

		// TODO: improve from just picking 1.  preferably with relevence.
		for (let i = 0; i < this.needs.length; i += 2) {
			if (isVar(this.needs[i]) || isVar(this.needs[i+1])) {
				// iterate all other options with this? just pick 1?
				// STUB
			} else {
				if (!ram.containsWith(this.needs[i], this.needs[i+1]))
					return []
			}
		}
		// fill variables STUB
	}
	// say I want would-meet apple
	// and I have needs apple
	// the easiest way to find this is to fill the needs in with what I have
}

// variables:
//         - create a place to store a value
//createvar = new FunctionData(
// 	'createvar',
// 	/*call*/(name) => {
// 		let ret = new StringData(name)
// 		ret.props.add('is','varspec')
//		return ret
// 	},
// 	/*relevence*/,
//// when do you want to create a variable?  when you need a variable and you have a name
// 	/*makes*/
//)
//         - place a value in such a place
//         - retrieve a value from such a place
// data-structures: (these are like promises of what to do and how to use something)
//         - make a class definition (collection of variables)
//         - initialize an object
//         - set/retrieve properties (variables)
// flow:
//         - trigger a different step than the next, conditionally
// procedures:
//         - collect a set of behaviors together
//         - trigger a collected set of behaviors and continue
// expressions:
//         - evaluate arithmetic on variables and constants

// sometimes how we get what we need depends on what we have
line2words = new FunctionData(
	'line2words',
	(line) => { // call
		let res = new ArrayData(line.split(' '), line)
		res.props.add('is','words')
		res.props.del('is','text-string')
		return res
	},
	['is','text-string'], // needs
	['is','word-array'] // makes
)

// makes text-string used by line2words.  example of small relevence habit
genproptextstring = new FunctionData(
	'genproptextstring',
	/*call*/(text) => {
		if (text.props.has('is','string')) {
			if (!text.props.has('is','text-string')) {
				text.props.add('is','text-string')
			}
			return text
		}
		return null
	},
	/*needs*/['is','text'],
	/*makes*/['is','text-string']
)

respondhello = new FunctionData(
	'respondhello',
	(words) => { // call
		console.log(words[0] + " world!")
		let res = new StringData("said " + words[0])
		res.props.add('is','output')
		res.props.add('is','text')
		return res
	},
	['is','hello-input'], // needs
	['is','output'] // makes
)

genprophelloinput = new FunctionData(
	'genprophelloinput',
	/*call*/(input) => {
		if (!input.props.has('is','words'))
			return null
		if (!input.props.has('is','hello-input')) {
			let x = input[0].toLowerCase()
			let c = x[x.length-1]
			if (c == ',' || c == '!' || c == '.') x = x.slice(0,x.length-1)
			if (x != 'hello' && x != 'hi') return null
			input.props.add('is','hello-input')
		}
		return input
	},
	/*needs*/['is','input'],
	/*makes*/['is','hello-input']
)

/* old, this isn't relevent to needed structure
// nodejs is missing a succinct read-line-from-stdin function.  make our own.
userinput = (() => {
	const readline = require('readline')
	const lines = []
	readline.createInterface({ input: process.stdin }).on('line', (line) => {
		lines.push(line)
	})

	let ret = new FunctionData(
		'userinput',
		(self) => { // call
			let res = new StringData(lines.shift())
			res.props.add('is','text')
			res.props.add('is','input')
			return res
		},
		() => { // relevence
			return lines.length > 0
		},
		['has','userinput-lines'] // needs
		['is','string','is','text','is','input'] // makes
	)
	ret.lines = lines
})()
genprophaslines = new FunctionData(
	'genprophaslines',
	(userinput) => { // call
	}
)
*/

// PATTERN REQUEST: we want to be able to store needed-steps.
// 	needed-steps are parallel needs that are needed for a helpful step
// 	any one may be met, and others are ignored once one is, but all are generated at once
// 	habits that meet needs expect them not to be inside an array and have no way of referring to that.
// 	[AND needs can be collapsed, but OR needs need patterns]
// 		[how do we pull a need out of the list?]

// is it useful to use as a need the memory generated by a habit?

// make a habit that plans core behavior
findstepsfor = new FunctionData(
	'findstepsfor',
	/*call*/(need, depth) => {
		// find a habit that does the final goal.
		// TODO: for now we assume all needs are 'is' and store just what-it-is in the need string.  when doesn't work anymore, make type-getters on Properties to retrieve the need details
		let ret = new ArrayData([])
		for (let habit of all_parts) {
			if (habit.makes.indexOf(need) !== -1) {
				// found a workable habit
				ret.push(habit.needs)
			}
		}
		ret.props.add('is','alternate-needs-array')
		return ret
	},
	// TODO: likely need more structure here
	// warning: MANY adds surrounding code complexity.  please put makes-interpretation in a habit if MANY is used. <- let's just put this in the class constructor for now
	/*needs*/['needs', VAR_X],
	/*makes*/[MANY, 'would-meet', VAR_X]
		// really we are usually met due to the need to meet a goal
		// and the need to have steps to meet that particular goal
		// how do we write a need that supports another need
		//
		// propose relevence is a list of reasons each with a strength, and greatest sum is chosen
		// HABIT/ACTIVE BEHAVIOR wants relevent-met-habits, to act on
		// PLANNING/GOAL BEHAVIOR wants relevent-unmet-needs to backtrace them towards relevent-met-needs
		//
		// we can meet any need with behavior, but it takes time.
		// 	[we are ignoring most of karl's needs]
		// 	[the reason is that an AI can meet them all]
		// 	AI meets some needs
		// 	AI is ongoing
		// 	therefore we are working on meeting 
		// ONE: we have a need for actively pursuing relevent needs
		// TWO: we need appropriate multitasking
		// 	AI is long term, so would happen after [quickly-]meetable-needs are met.
		//
		// make popcorn
		// need: made-popcorn
		// 1. identify that we need unmade popcorn and a microwave
		// ^-- made-popcorn is how? we can meet any need with behavior
		// 		what is substep of find-path-to-need
		// 			find path-part-to-need
		//
		// need: to meet needs
		// need: some-need
		// makestepsfor makes: some-other-need tagged makes: some-need
		//
		// when we push some-need we want to stimulate pushing some-other-need.
		//
		// use a variable as the link target, and use e.g. met-need
		// as the link type
)

// TODO: add structure enough to build findstepsfor
// TODO: build findstepsfor
// TODO 2: add to surrounding process: remove source needs when there is no longer a reason for their presence


all_parts = [ line2words, genproptextstring, respondhello, genprophelloinput ]

ram = new ActiveMemory()
//ram.add(ram)
optstried = new Set()
steps = []

var readline = require('readline')
readline.createInterface({
	input: process.stdin
}).on('line', (line) => {
	line = new StringData(line)
	line.props.add('is','input')
	line.props.add('is','text')
	ram.add(line)

	let cont
	do {
		cont = false
		for (let part of all_parts) {
			//console.log('for-loop-of-parts ' + part.name)
			// TODO: >1 parameter,must redo next line
			let rel = ram.getWith(...part.needs)
			if (rel) {
				for (let a of rel) {
					//console.log('for-loop-of-rel ' + part.name + ' ' + a)
					if (optstried.has(part.name + ' ' + a)) continue; 
					//console.log('call-part ' + part.name + ' ' + a)
					res = part.call(a)
					optstried.add(part.name + ' ' + a)
					if (res == null) continue
					// TODO: res==null is considered failure now
					ram.add(res)
					//console.log('part-called')
					step = [a, part.name, res]
					steps.push(step)
					console.log('made ' + step)
					cont = true
				}
			}
		}
	} while (cont)
})


// it looks like there is a reason to store each item in active memory.
// one reason is that it is needed by a step in a process.
// when there are no reasons, it is removed.
// using an item should link it to what it is used by, so that it may be found easier again when needed

// Please grow to understand your own structure.  What is needed to make an intellect?
// 	^-- we want it to learn the basic needs of life, eventually, on its own.
// 	    notably those to do with care for others.

// MEMORY: see MEMORY above

// RELEVENCE: the gofer.  Provides the link between needs, etc, and the behavior that
// finds them.  Is relevent for meeting needs in active ways and likely much else.

// BEHAVIOR: the core.  combines interdependent habits from memory to meet needs.
// The two below go on simultaneously.  only 1 is needed.
// A way to plan behavior:
// 	find habits that do the final goal, discern their needs
// 	pick needs to treat as a new final goal and repeat until needs are met
// A way to make behavior:
// 	find a habit that does a goal
// 	find with this process, information that meets its needs <spawns more>
// 	run it
// 		TODO: for subprocesses spawning,
//		consider labelling things that are very quick and side-effect-free
//		these can probably be immediately run.
//		
