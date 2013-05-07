var watchdog = require('builtin/watchdog'),
	process = require('builtin/process'),
	console = require('console');

function main() {
	console.log('Testing watchdog for 5 seconds');
	watchdog.set(5);
	process.sleep(6);
	console.log('something went wrong')
}

