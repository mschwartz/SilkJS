function foo_action() {
	res.write('foo_action\n');
	res.stop();
}

function bar_action() {
	res.write('bar_action\n');
	res.stop();
}

function main_action() {
	var console = require('console');
	console.dir(req);
	console.dir(res);
	res.write('/ requested\n');
	res.stop();
}

function notFound_action() {
	res.write('My 404 handler\n');
	res.stop();
}

