let tokenre = /^Unexpected token (.*)$/

function evaluate(idea)
{
	try {
		return eval(idea)
	} catch (e) {
		if (e.name !== 'SyntaxError') { throw e }
		let token = e.message.match(tokenre)
		if (token) {
			e.problem = token[1]
			e.problemoffset = 0
		} else if (e.message === 'Unexpected identifier') {
			e.problem = /[^a-zA-Z0-9_][a-zA-Z_]/
			e.problemoffset = 1
		} else if (e.message === 'Unexpected string') {
			e.problem = /[^'"]'"/
			e.problemoffset = 1
		}
		throw e
	}
}

module.exports = evaluate
