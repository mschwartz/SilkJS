Config = {
	port: 9090,
	numChildren: 500,
	requestsPerChild: 100000,
	documentRoot: 'httpd/docroot',
	lockFile: '/tmp/silkf.lock',
	mysql: {
		host: 'localhost',
		user: 'mschwartz',
		passwd: '',
		db: 'sim'
	}
};
