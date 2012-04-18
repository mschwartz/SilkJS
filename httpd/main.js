#!/usr/local/bin/silkjs
// httpd/main.js

print_r = require('builtin/print_r');
console = require('console');
fs = require('fs');

LogFile = require('LogFile');
net = require('builtin/net');
process = require('builtin/process');
v8 = require('builtin/v8');
http = require('builtin/http');
buffer = require('builtin/buffer');

uglify = require('UglifyJS/uglify-js');

//include('lib/phpjs.js');
include('lib/forEach.js');
//include('lib/Exceptions.js');
include('lib/Util.js');
Json = require('Json');
include('lib/Jst.js');
include('lib/Showdown.js');
MySQL = require('MySQL');
var SSH = require('SSH');
////
include('httpd/config.js');
include('httpd/request.js');
include('httpd/response.js');
include('httpd/child.js');

function main() {
    var debugMode = false;
    // load any user provided JavaScripts
	arguments.each(function(arg) {
		if (arg === '-d') {
            debugMode = true;
			return false;
        }		
	});

	if (debugMode) {
		v8.enableDebugger();
		log('Running in debug mode');
	}
    arguments.each(function(arg) {
        if (arg.endsWith('.js') || arg.endsWith('.coffee')) {
            include(arg);
        }
    });

	var pid;
    var fd = fs.open(Config.lockFile, fs.O_WRONLY|fs.O_CREAT|fs.O_TRUNC, 0644);
    fs.close(fd);
    var serverSocket = net.listen(Config.port, 10, Config.listenIp);

    global.logfile = new LogFile(Config.logFile || '/tmp/httpd-silkjs.log');
    
    if (debugMode) {
		while (1) {
	        HttpChild.run(serverSocket, process.getpid());
		}
    }
    
    var children = {};
    for (var i=0; i<Config.numChildren; i++) {
        pid = process.fork();
        if (pid == 0) {
            HttpChild.run(serverSocket, process.getpid());
            process.exit(0);
        }
        else if (pid == -1) {
            console.error(process.error());
        }
        else {
            children[pid] = true;
        }
    }

    var logMessage = 'SilkJS HTTP running with ' + Config.numChildren + ' children on port ' + Config.port;
    if (Config.listenIp !== '0.0.0.0') {
        logMessage += ' on IP ' + Config.listenIp;
    }
//    console.log('Silk running with ' + Config.numChildren + ' children on port ' + Config.port);
//    logfile.write('Silk running with ' + Config.numChildren + ' children on port ' + Config.port + '\n');
    console.log(logMessage);
    logfile.writeln(logMessage);
    while (true) {
        var o = process.wait();
        if (!children[o.pid]) {
console.log('********************** CHILD EXITED THAT IS NOT HTTP CHILD');
            continue;
        }
        delete children[o.pid];
        pid = process.fork();
        if (pid == 0) {
            HttpChild.run(serverSocket, process.getpid());
            process.exit(0);
        }
        else if (pid == -1) {
            console.error(process.error());
        }
        else {
            children[pid] = true;
        }
    }
}
