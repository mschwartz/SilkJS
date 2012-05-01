// httpd/config.js

/*
 * This is the default Config object for the HTTP server.  You would override these values in your WWW App's bootstrap.js file.
 * 
 * For example (bootstrap.js):
 * 
 * Config.port = 8080; // listen on 8080 instead of 9090
 * Config.numChildren = 50; // start and maintain 50 HttpChild processes.
 * Or:
 * 
 * Config.extend({
 *      port: 8080, // listen on 8080 instead of 9090
 *      numChildren: 50 // start and maintain 50 HttpChild processes
 * });
 */
var fs = require('builtin/fs');
Config = {
	port: 9090,
	numChildren: 50,
	requestsPerChild: 100000,
    listenIp: '0.0.0.0',    // listen socket will be bound to this IP.  '0.0.0.0' means ANY IP on this machine.
	documentRoot: fs.exists('httpd/docroot') ? 'httpd/docroot' : '/usr/share/silkjs/httpd/docroot',
    logFile: '/tmp/httpd-silkjs.log',
	directoryIndex: [
		'index.sjs',
		'index.jst',
		'index.coffee',
		'index.md',
		'index.html'
	],
    // you can set up a default MySQL credentials/db here.  The HttpChild will automatically create a connection, if this is configured, as global SQL variable.
//	mysql: {
//		host: 'localhost',
//		user: 'xxx',
//		passwd: 'xxx',
//		db: 'xxx'
//	},
	lockFile: '/tmp/silkf.lock'
};
