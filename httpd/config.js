// httpd/config.js
var fs = require('builtin/fs');
Config = {
	port: 9090,
	numChildren: 250,
	requestsPerChild: 100000,
	documentRoot: fs.exists('httpd/docroot') ? 'httpd/docroot' : '/usr/share/silkjs/httpd/docroot',
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
