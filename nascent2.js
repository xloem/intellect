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
			for (let b of props)
				if (! a.has(b))
					continue outer
			return true
		}
		return false
	}
	getWith(...props) {
		let ret = []
		outer: for (let a of this.ram) {
			for (let b of props)
				if (! a.has(b))
					continue outer
			ret.push(a)
		}
		if (ret.length > 0) return ret
		return null
	}
}
class StringData extends String {
	constructor(str, from) {
		super(str)
		this.props = new Set(from && from.props)
		this.add('string')
		this.del('array')
	}
	add(val) {
		this.props.add(val)
	}
	has(val) {
		return this.props.has(val)
	}
	del(val) {
		this.props.delete(val)
	}
}
class ArrayData extends Array {
	constructor(arr, from) {
		super(...arr)
		this.props = new Set(from && from.props)
		this.add('array')
		this.del('string')
	}
	add(val) {
		this.props.add(val)
	}
	has(val) {
		return this.props.has(val)
	}
	del(val) {
		this.props.delete(val)
	}
}

function line2words(line) {
	let res = new ArrayData(line.split(' '), line)
	res.add('words')
	return res
}
line2words.relevence = function(ram) {
	return ram.getWith('text', 'string')
}
line2words.makes = ['array','words'] // property values produced

function respondhello(words) {
	console.log(words[0] + ", user!")
	let res = new StringData("said " + words[0])
	res.add('output')
	res.add('text')
	return res
}
respondhello.relevence = function(ram) {
	let res = ram.getWith('words', 'input')
	if (res) res = res.filter(x => { x = x[0].toLowerCase(); return x == 'hello' || x == 'hi'})
	return res
}
respondhello.makes = ['output']

// nodejs is missing a succinct read-line-from-stdin function.  here, we make one.
userinput = (() => {
	const readline = require('readline')
	const lines = []
	readline.createInterface({ input: process.stdin }).on('line', (line) => {
		lines.push(line)
	})

	function userinput() {
		let res = new StringData(lines.shift())
		res.add('text')
		res.add('input')
		return res
	}
	userinput.relevence = function() {
		return lines.length > 0
	}
	userinput.makes = ['string','text','input']

	return userinput
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
	line.add('input')
	line.add('text')
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
					res = part(a)
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
