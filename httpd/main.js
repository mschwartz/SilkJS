#!/usr/local/bin/silkjs
// httpd/main.js

print_r = require('builtin/print_r');
console = require('console');
fs = require('fs');

LogFile = require('LogFile');
net = require('builtin/net');
process = require('builtin/process');
async = require('builtin/async');
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
var SSH = require('SSH');
////
include('httpd/config.js');
include('httpd/Server.js');
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
    if (Config.mysql) {
        MySQL = require('MySQL').MySQL;
    }

    var pid;
    var fd = fs.open(Config.lockFile, fs.O_WRONLY|fs.O_CREAT|fs.O_TRUNC, parseInt('0644', 8));
    fs.close(fd);
    var serverSocket = net.listen(Config.port, 10, Config.listenIp);
    global.logfile = new LogFile(Config.logFile || '/tmp/httpd-silkjs.log');
    
    Server.onStart();

    if (debugMode) {
        while (1) {
            HttpChild.run(serverSocket, process.getpid());
        }
    }
   
    var children = {},
        children_map = {},
        children_fifo = [];
    function forkChild() {
        var pair = net.socketpair();
        pid = process.fork();
        if (pid === 0) {
            HttpChild.run(serverSocket, process.getpid(), pair[1]);
            process.exit(0);
        }
        else if (pid == -1) {
            console.error(process.error());
        }
        else {
            children_map[pair[0]] = children[pid] = { pid: pid, control: pair[0], status: 'f' };
            children_fifo.push(children[pid]);
        }
    }

    for (var i=0; i<Config.numChildren; i++) {
        forkChild();
    }
    var logMessage = 'SilkJS HTTP running with ' + Config.numChildren + ' children on port ' + Config.port + ' from documentRoot ' + Config.documentRoot;
    if (Config.listenIp !== '0.0.0.0') {
        logMessage += ' on IP ' + Config.listenIp;
    }
//    console.log('Silk running with ' + Config.numChildren + ' children on port ' + Config.port);
//    logfile.write('Silk running with ' + Config.numChildren + ' children on port ' + Config.port + '\n');
    console.log(logMessage);
    logfile.writeln(logMessage);


    var readfds = async.alloc_fd_set();
    var maxfd;
    function build_fd_set() {
        async.FD_ZERO(readfds);
        async.FD_SET(serverSocket, readfds);
        maxfd = serverSocket;
        children.each(function(child) {
            var fd = child.control;
            if (fd > maxfd) {
                maxfd = fd;
            }
            async.FD_SET(fd, readfds);
        });
    }

    var child;
    function processInput(fd) {
        if (fd === serverSocket) {
            return;
        }
        var child = children_map[fd];
        if (!child) {
            console.log('processInput no child ' + fd);
            return;
        }
        child.status = async.read(child.control, 1);
        wakeupChild();
    }
    var wakeupCount = 0;
    function wakeupChild() {
        if (!wakeupCount) {
            return;
        }
        var n,
            len = children_fifo.length;
        for (n=0; n<len; n++) {
            var child = children_fifo.shift();
            children_fifo.push(child);
            if (child.status === 'r') {
                net.write(child.control, 'g', 1);
                child.status = 'x';
                wakeupCount--;
                return;
            }
        }
        // console.log('no children');
    }
    function removeChild(child) {
        var control = child.control;
        delete children_map[control];
        delete children[child.pid];
        var new_fifo = [];
        children_fifo.each(function(e) {
            if (e.control !== control) {
                new_fifo.push(e);
            }
        });
        children_fifo = new_fifo;
    }
    while (true) {
        build_fd_set();
        var o = async.select(maxfd+1, readfds);
        if (o === false) {
            while ((child = process.wait(true))) {
                console.log('child ' + child.pid + ' exited');
                if (!children[child.pid]) {
                    console.log('********************** CHILD EXITED THAT IS NOT HTTP CHILD');
                    continue;
                }
                removeChild(child);
                forkChild();
            }
        }
        else {
            // console.dir(o);
            o.read.each(processInput);
            if (o.read.indexOf(serverSocket) !== -1) {
                wakeupCount++;
            }
            if (wakeupCount) {
                wakeupChild();
            }
        }
    }
}
