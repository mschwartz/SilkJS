// httpd/config.js
var fs = require('builtin/fs');
Config = {
	port: 9090,
	numChildren: 20,
	requestsPerChild: 100,
    listenIp: '10.0.0.182', // '0.0.0.0',
	documentRoot: fs.exists('httpd/docroot') ? 'httpd/docroot' : '/usr/share/silkjs/httpd/docroot',
    logFile: '/tmp/httpd-silkjs.log',
	directoryIndex: [
		'index.sjs',
		'index.jst',
		'index.coffee',
		'index.md',
		'index.html'
	],
//	mysql: {
//		host: 'localhost',
//		user: 'mschwartz',
//		passwd: '',
//		db: 'sim'
//	},
	lockFile: '/tmp/silkf.lock'
};
