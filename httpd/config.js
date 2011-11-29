Config = {
	port: 9090,
	numChildren: 250,
	requestsPerChild: 100000,
	documentRoot: fs.exists('httpd/docroot') ? 'httpd/docroot' : '/usr/share/SilkJS/httpd/docroot',
	lockFile: '/tmp/silkf.lock',
	mysql: {
		host: 'localhost',
		user: 'mschwartz',
		passwd: '',
		db: 'sim'
	}
};
