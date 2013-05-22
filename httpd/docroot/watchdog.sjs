var watchdog = require('builtin/watchdog'),
	console = require('console'),
	process = require('builtin/process');

console.log('setting watchdog to 5 seconds');
watchdog.set(5);
console.log('sleeping 6 seconds');
process.sleep(6);
console.log('should not get here');

res.write('watchdog test failed');  // should never render
res.stop();							// should never get here
