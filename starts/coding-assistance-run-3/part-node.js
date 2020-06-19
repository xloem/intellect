// let's do load-and-store
// sets of links with numbers

if (!global.count_uuid) {
	global.count_uuid = 0
}
let result = {
	uuid: global.count_uuid,
	links: {},
	data: {}
}
global.count_uuid = global.count_uuid + 1
