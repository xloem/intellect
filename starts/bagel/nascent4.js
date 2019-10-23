class ActiveMemory {
	constructor() {
		this.ram = []
		this.cloned = false
	}
	clone() {
		let ret = new ActiveMemory();
		ret.ram = this.ram
		ret.cloned = true
		return ret
	}
	add(...items) {
		if (this.cloned) { this.ram = this.ram.slice(); this.cloned = false; }
		for (let item of items)
			this.ram.push(item)
	}
	del(...items) {
		if (this.cloned) { this.ram = this.ram.slice(); this.cloned = false; }
		for (let item of items) {
			let index = this.ram.indexOf(item)
			this.ram.splice(index, 1)
		}
	}
	contains(...items) {
		for (let item of items)
			if (this.ram.indexOf(item) === -1) return false
		return true
	}
	containsWith(...props) {
		let ret = []
		outer: for (let a of this.ram) {
			for (let i = 0; i < props.length; i += 2)
				if (! a.props.has(props[i], props[i+1]))
					continue outer
			return true
		}
		return false
	}
	getWith(...props) {
		let ret = []
		outer: for (let a of this.ram) {
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
class Property {
	constructor(type, dest) {
		this.type = type
		this.dest = dest
	}
}
class Properties {
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
	has(type, dest) {
		for (let p of this.props)
			if (p.type == type && p.dest == dest)
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
class FunctionData {
	constructor(name, func, relevence, makes) {
		this.name = name
		this.props = new Properties('function')
		this.props.add('name', name)
		this.call = func
		this.relevence = relevence
		this.makes = makes
	}
}

// variables:
//         - create a place to store a value
// createvar = new FunctionData(
// 	'createvar',
// 	/*call*/(name) => {
// 		let ret = new StringData(name)
// 		ret.add
// 		let name = obj_and_name[1]
// 	},
// 	/*relevence*/,
// 	/*makes*/
// )
//         - place a value in such a place
//         - retrieve a value from such a place
// data-structures:
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

line2words = new FunctionData(
	'line2words',
	(line) => { // call
		let res = new ArrayData(line.split(' '), line)
		res.props.add('is','words')
		return res
	},
	(ram) => { // relevence
		return ram.getWith('is','text', 'is','string')
	},
	['is','array','is','words'] // makes
)

respondhello = new FunctionData(
	'respondhello',
	(words) => { // call
		console.log(words[0] + ", user!")
		let res = new StringData("said " + words[0])
		res.props.add('is','output')
		res.props.add('is','text')
		return res
	},
	(ram) => { // relevence
		let res = ram.getWith('is','words', 'is','input')
		if (res) res = res.filter(x => { x = x[0].toLowerCase(); return x == 'hello' || x == 'hi'})
		return res
	},
	['is','output', 'is','text'] // makes
)

// nodejs is missing a succinct read-line-from-stdin function.  make our own.
userinput = (() => {
	const readline = require('readline')
	const lines = []
	readline.createInterface({ input: process.stdin }).on('line', (line) => {
		lines.push(line)
	})

	return new FunctionData(
		'userinput',
		() => { // call
			let res = new StringData(lines.shift())
			res.props.add('is','text')
			res.props.add('is','input')
			return res
		},
		() => { // relevence
			return lines.length > 0
		},
		['is','string','is','text','is','input'] // makes
	)
})()


all_parts = [ line2words, respondhello ]

ram = new ActiveMemory()
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
			let rel = part.relevence(ram)
			if (rel) {
				for (let a of rel) {
					//console.log('for-loop-of-rel ' + part.name + ' ' + a)
					if (optstried.has(part.name + ' ' + a)) continue; 
					//console.log('call-part ' + part.name + ' ' + a)
					res = part.call(a)
					ram.add(res)
					//console.log('part-called')
					step = [a, part.name, res]
					steps.push(step)
					console.log('made ' + step)
					cont = true
					optstried.add(part.name + ' ' + a)

				}
			}
		}
	} while (cont)
})
