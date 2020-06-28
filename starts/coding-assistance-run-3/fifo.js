const fs = require('fs')
const net = require('net')

module.exports = function(path)
{
	const fd = fs.openSync(path, fs.constants.O_RDWR | fs.constants.O_NONBLOCK)
	return new net.Socket({ fd })
}
