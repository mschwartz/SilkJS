#!/usr/local/bin/silkjs
// httpd/main.js

include('lib/coffee-script.js');


print_r = require('builtin/print_r');
console = require('console');
fs = require('builtin/fs');
logfile = require('builtin/logfile');
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
include('lib/Json.js');
include('lib/Jst.js');
include('lib/Showdown.js');
MySQL = require('MySQL.js').MySQL;
include('lib/ssh.js');
////
include('httpd/config.js');
include('httpd/request.js');
include('httpd/response.js');
include('httpd/child.js');

app = {
    serverDir: fs.getcwd()
};

function main() {
    var debugMode = false;
    // load any user provided JavaScripts
    arguments.each(function(arg) {
        if (arg.endsWith('.js') || arg.endsWith('.coffee')) {
            include(arg);
        }
        else if (arg === '-d') {
            debugMode = true;
        }
    });
    var pid;
    var fd = fs.open(Config.lockFile, fs.O_WRONLY|fs.O_CREAT|fs.O_TRUNC, 0644);
    fs.close(fd);
    var serverSocket = net.listen(Config.port);

    logfile.init();
    
    if (debugMode) {
        log('Running in debug mode');
		v8.enableDebugger();
        HttpChild.run(serverSocket, process.getpid());
        exit(0);
    }
    
    pid = process.fork();
    if (pid == 0) {
        while (1) {
            process.sleep(5);
            logfile.flush();
        }
    }
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
