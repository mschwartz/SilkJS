#!/usr/local/bin/SilkJS
// httpd/main.js

//v8.gc();

silk = global.silk || {};
silk.includes = {};
silk.include = function(fn) {
    silk.includes[fn] = fs.fileModified(fn);
    include(fn);
};

silk.checkIncludes = function() {
    var reload = false;
    forEach(silk.includes, function(mtime, fn) {
        var newMtime = fs.fileModified(fn);
        if (newMtime > mtime) {
            silk.includes[fn] = newMtime;
            reload = true;
        }
    });
    if (reload) {
        logfile.write('reload\n');
        forEach(silk.includes, function(mtime, fn) {
            include(fn);
        });
    }
};

include('lib/string.js');
include('lib/object.js');
include('lib/phpjs.js');
include('lib/print_r.js');
include('lib/forEach.js');
include('lib/Exceptions.js');
include('lib/Util.js');
include('lib/Jst.js');
include('lib/MySQL.js');
include('lib/Server.js');
include('lib/Schema.js');

include('httpd/config.js');
include('httpd/request.js');
include('httpd/response.js');
include('httpd/child.js');

function main() {
	// load any user provided JavaScripts
	forEach(arguments, function(arg) {
		if (arg.endsWith('.js')) {
			silk.include(arg);
		}
	});
    var pid;
    var fd = fs.open(Config.lockFile, fs.O_WRONLY|fs.O_CREAT|fs.O_TRUNC, 0644);
    fs.close(fd);
    var serverSocket = net.listen(Config.port);
	// global.acceptLock = sem.init();

	logfile.init();
	pid = process.fork();
	if (pid == 0) {
		while (1) {
			process.sleep(5);
			logfile.flush();
		}
	}
//	log('main ' + print_r(acceptLock));
    for (var i=0; i<Config.numChildren; i++) {
        pid = process.fork();
        if (pid == 0) {
            HttpChild.run(serverSocket, process.getpid());
            process.exit(0);
        }
        else if (pid == -1) {
            console.error(process.error());
        }
    }
    Server.onStart();
	log('Silk running with ' + Config.numChildren + ' children on port ' + Config.port);
	logfile.write('Silk running with ' + Config.numChildren + ' children on port ' + Config.port);
	while (true) {
		var o = process.wait();
        pid = process.fork();
        if (pid == 0) {
            HttpChild.run(serverSocket, process.getpid());
            process.exit(0);
        }
        else if (pid == -1) {
            console.error(process.error());
        }
    }
}
