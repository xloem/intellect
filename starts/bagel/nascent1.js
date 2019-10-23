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
	add(item) {
		if (this.cloned) { this.ram = this.ram.slice(); this.cloned = false; }
		this.ram.push(item)
	}
	del(item) {
		if (this.cloned) { this.ram = this.ram.slice(); this.cloned = false; }
		let index = this.ram.indexOf(item)
		this.ram.splice(index, 1)
	}
	contains(item) {
		for (let a of this.ram)
			if (a == item) return a
		return false
	}
	containsWith(prop, val) {
		let ret = []
		for (let a of this.ram)
			if (a[prop] == val)
				return true
		return false
	}
	getWith(prop, val) {
		let ret = []
		//console.log('get-with ' + prop + ' ' + val)
		for (let a of this.ram) {
			//console.log(a + ' ' + prop + ' is ' + a[prop])
			if (a[prop] == val)
				ret.push(a)
		}
		if (ret.length > 0) return ret
		return null
	}
}

function sayhi() {
	res = 'hi'
	res.use = 'output'
	console.log(res)
	return 'said ' + res
}
sayhi.relevence = function(ram) {
	return [ram.ram[0]];
}

function line2words(line) {
	let res = line.split(' ')
	res.type = 'list'
	return res
}
line2words.relevence = function(ram) {
	return ram.getWith('type', 'text')
}

all_parts = [ line2words, sayhi ]

ram = new ActiveMemory()
optstried = new Set()
steps = []

var readline = require('readline')
readline.createInterface({
	input: process.stdin
}).on('line', (line) => {
	line = new String(line)
	line.use = 'input'
	line.type = 'text'
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
					//console.log('part-called')
					step = [a, part.name, res]
					steps.push(step)
					console.log('made ' + step)
					cont = true
					optstried.add(part.name + ' ' + a)

				}
			}
			//if (rel) {
			//	let ram2 = ram.clone()
			//	for 
			//}
		}
	} while (cont)
})
