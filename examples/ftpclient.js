#!/usr/local/bin/silkjs

var FTP = require('FTP'),
    ReadLine = require('ReadLine'),
    fs = require('fs'),
    process = require('builtin/process'),
    ncurses = require('builtin/ncurses'),
    editline = require('builtin/editline'),
    console = require('console');

var ftp,
    alive = true,
    stdin;

var commands = {
    '!': {
        help: '!command - execute local command',
        fn: function(args) {
            console.log(process.exec(args));
        }
    },
    pasv: {
        help: 'pasv - set mode to passive',
        fn: function(args) {
            ftp.pasv();
            console.log(ftp.lastResponse());
        }
    },
    active: {
        help: 'active - set mode to active',
        fn: function(args) {
            ftp.active();
            console.log(ftp.lastResponse());
        }
    },
    cd: {
        help: 'cd [path] - change remote directory to path',
        fn: function(args) {
            ftp.chdir(args);
            console.log(ftp.lastResponse());
            stdin.prompt(ftp.cwd + '> ');
        }
    },
    lcd: {
        help: 'lcd [path] - change local directory to path',
        fn: function(args) {
            fs.chdir(args);
            console.log('local directroy now ' + fs.getcwd());
        }
    },
    dir: {
        help: 'dir [path] - get remote directory listing',
        fn: function(args) {
            var dir = ftp.dir(args);
            console.log(ftp.lastResponse());
            console.log(dir);
        }
    },
    list: {
        help: 'list [path] - get terse remote directory listing',
        fn: function(args) {
            console.dir(ftp.list(args));
        }
    },
    stat: {
        help: 'stat path - get info about file or directory',
        fn: function(args) {
            console.dir(ftp.stat(args));
        }
    },
    mkdir: {
        help: 'mkdir path - create a directory on remote server',
        fn: function(args) {
            ftp.mkdir(args);
            console.log(ftp.lastResponse());
        }
    },
    rmdir: {
        help: 'rmdir path - delete a directory on remote server',
        fn: function(args) {
            ftp.rmdir(args);
            console.log(ftp.lastResponse());
        }
    },
    get: {
        help: 'get remotefile [localfile] - retrive a file from server',
        fn: function(args) {
            args = args.split(/\s+/);
            var remoteFile = args[0],
                localFile = args[1];
            if (!localFile) {
                localFile = remoteFile.split('/');
                localFile = localFile.pop();
            }
            ftp.getFile(remoteFile, localFile);
            console.log(ftp.lastResponse());
        }
    },
    put: {
        help: 'put localfile [remotefile] - send a file to the server',
        fn: function(args) {
            args = args.split(/\s+/);
            var localFile = args[0],
                remoteFile = args[1];
            if (!remoteFile) {
                remoteFile = localFile.split('/');
                remoteFile = remoteFile.pop();
            }
            ftp.putFile(localFile, remoteFile);
            console.log(ftp.lastResponse());
        }
    },
    cat: {
        help: 'cat remoteFiel - type a remote file to screen',
        fn: function(args) {
            console.log(ftp.readFile(args));
        }
    },
    quit: {
        help: 'quit - exit the program',
        fn: function(args) {
            alive = false;
        }
    },
    help: {
        help: 'help - print this help',
        fn: function(args) {
            if (args && commands[args]) {
                console.log(commands[args].help);
            }
            else {
                commands.each(function(command) {
                    console.log(command.help);
                });
            }
        }
    }
};

function main(host, username, password) {
    if (!host) {
        println("Usage: ftpclient.js host [username [password]]");
        return;
    }
    if (!username) {
        username = process.getlogin();
    }
    if (!password) {
        password = console.getPassword('Enter password for ' + username + ': ');
    }

    stdin = new ReadLine('ftpclient'); // editline.init('ftpclient');
    ftp = new FTP(host, username, password);
    console.log(ftp.lastResponse());
    console.log('Current directory is ' + ftp.cwd);
    stdin.prompt(ftp.cwd + '> ');
    while (alive) {
        var command = stdin.gets();
        if (!command) {
            break;
        }
        var args = command.trim().split(/\s+/);
        var cmd = args.shift();
        if (cmd[0] === '!') {
            if (cmd.length > 1) {
                args.unshift(cmd.substr(1));
                cmd = '!';
            }
            else {
                cmd = '';
            }
        }
        args = args.join(' ');
        if (commands[cmd]) {
            commands[cmd].fn(args);
        }
        else {
            console.log(command);
            console.log('Unknown command.  Type "help" for help');
        }
    }
    stdin.close();
}
