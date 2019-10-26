/* NEXT: add aux func
         store access order tree
      -> make nodes objects */
/* BTW: need to practice slowly or will have error when fast <- although could have been a 'trick'; clocks */
async function learn1(name, goal, knowledge)
{
	let focus = [{val: name}]
	knowledge[name] = {val: focus}
	console.log("Ctx: " + name)

	a = function(idea)
	{
		focus.push({val: idea})
		//focusUpdated()
	}
	
	randomSeed(0)

	function pickNextIdx(lastIdx, focus)
	{  // make this function be a contextual property
		return Math.floor(random() * (focus.length - lastIdx)) + lastIdx
	}

	async function trial()
	{
		let result = []
		let last = [{focus: focus, idx: 0}]
		while (true) {
			let llast = last[last.length - 1]
			let next = pickNextIdx(llast.idx, llast.focus)
			if (next >= llast.focus.length) {
				if (random() >= 0.5) {
					last.pop()
				} else {
					llast.idx = 0
				}
				if (last.length == 0) {
					break
				}
				continue
			}
			llast.idx = next
			next = llast.focus[idx].val
			if (next in knowledge && random() < 0.5) {
				// open subcontext or subtree
				last.push({focus: knowledge[next].val, idx:0})
				continue
			}
			result.push(next)
		}
		let resstr = result.join('')
		return await goal(resstr)
	}
	// TODO: call trial in loop that checks return value
}
