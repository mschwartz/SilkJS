// httpd/config.js
var fs = require('builtin/fs');
Config = {
	port: 9090,
	numChildren: 20,
	requestsPerChild: 100,
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
