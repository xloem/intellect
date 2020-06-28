const fs = require('fs')

global.count_uuid = global.count_uuid + 1

nodes = {
	_count: 0,
	_store: {},
	_make: function() {
		return {
			link: function(how, to) {
				if (typeof(how) !== 'number') {
					how = how.uuid
				}
				if (typeof(to) !== 'number') {
					to = to.uuid
				}
				this.links[how] = to
			},
			get: function(how) {
				if (typeof(how) !== 'number') {
					how = how.uuid
				}
				return this.links[how]
			},
			save: function() {
				fs.writeFileSync('nodes/' + this.uuid + '.node',
					JSON.stringify({links:this.links,data:this.data}))
			}
		}
	},
	get: function(uuid) {
		if (uuid === this._count) {
			return make()
		}
		if (uuid in this._store) {
			return this._store[uuid];
		}
		const data = JSON.parse(fs.readFileSync('nodes/' + uuid + '.node'))
		const result = this._make()
		result.uuid = uuid
		result.links = data.links
		result.data = data.data
		this._store[uuid] = result
		return result
	},
	make: function() {
		const result = this._make()
		this._count ++
		result.uuid = this._count
		result.links = {}
		return result
	},
	load: function() {
		this.clean()
		this._count = int(fs.readFileSync('nodes/node_count'))
	},
	save: function() {
		for (let node of this._store) {
			node.save()
		}
		fs.writeFileSync('node/node_count', this._count)
	},
	clean: function() {
		this._store = {}
	}
}
module.exports = nodes
nodes.load()

// can put constant nodes here
nodes.const = {
	root: nodes.get(0)
}
