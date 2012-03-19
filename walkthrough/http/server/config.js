Config.extend({
	port: 9091,					// listen on port 9091
	numChildren: 25,			// pre-fork this many child processes
	documentRoot: 'docroot'		// path to documentRoot
});

function foo_action() {
	println('foo_action');
}

