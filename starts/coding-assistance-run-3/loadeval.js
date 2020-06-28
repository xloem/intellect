const fs = require('fs')

module.export = function(name)
{
	const code = fs.readFileSync('part-' + name + '.js')
	return eval(code)
}
