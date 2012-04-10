#!/usr/local/bin/silkjs

var GitHub = require('GitHub').GitHub,
    ReadLine = require('ReadLine'),
    process = require('builtin/process'),
    fs = require('fs'),
    console = require('console');
    
var gh,
    stdin,
    alive = true;

function defaultPrompt() {
    stdin.prompt('GitHub:' + gh.user.login + '> ');
}

var commands = {
    user: {
        help: 'user [username] - get information about user.',
        fn: function(username) {
            console.dir(gh.getUser(username));
        }
    },
    editUser: {
        help: 'editUser - edit your user information.',
        fn: function(username) {
            var o = {};
            console.log('Enter your information below.  Hit ^C to abort.');
            try {
                ['name', 'email', 'blog', 'company', 'location', 'hireable', 'bio'].each(function(s) {
                    stdin.prompt(s + ' (' + gh.user[s] + '): ');
                    var inp = stdin.gets();
                    if (!inp) {
                        alive = false;
                        return false;
                    }
                    o[s] = inp.trim();
                    if (!o[s].length) {
                        o[s] = gh.user[s];
                    }
                });
            }
            catch (e) {
                defaultPrompt();
                console.log('*** Aborted');
                return;
            }
            defaultPrompt();
            gh.editUser(o);
        }
    },
    listEmails: {
        help: 'listEmails - list your email addresses.',
        fn: function() {
            console.dir(gh.getEmails());
        }
    },
    addEmails: {
        help: 'addEmail email_address[ email_address...] - add email_addresses to your list of email addresses.',
        fn: function(args) {
            var addresses = args.split(/\s+/);
            console.dir(gh.addEmails(addresses));
        }
    },
    deleteEmails: {
        help: 'deleteEmail email_address[ email_address...] - delete email_addresses from your list of email addresses.',
        fn: function(args) {
            var addresses = args.split(/\s+/);
            console.dir(gh.deleteEmails(addresses));
        }
    },
    listFollowers: {
        help: 'listFollowers [username] - list followers for user.',
        fn: function(args) {
            console.dir(gh.listFollowers(args));
        }
    },
    listFollowing: {
        help: 'listFollowing [username] - list who a user is following.',
        fn: function(args) {
            console.dir(gh.listFollowing(args));
        }
    },
    amFollowing: {
        help: 'amFollowing username - check if you are following a user',
        fn: function(args) {
            if (!args || !args.length) {
                console.log('*** command error - you must supply a username');
                return;
            }
            console.dir(gh.amFollowing(args));
        }
    },
    follow: {
        help: 'follow username - follow a user',
        fn: function(args) {
            if (!args || !args.length) {
                console.log('*** command error - you must supply a username');
                return;
            }
            console.dir(gh.follow(args));
        }
    },
    unfollow: {
        help: 'unfollow username - unfollow a user',
        fn: function(args) {
            if (!args || !args.length) {
                console.log('*** command error - you must supply a username');
                return;
            }
            console.dir(gh.unfollow(args));
        }
    },
    listKeys: {
        help: 'listKeys - list public keys.',
        fn: function() {
            console.dir(gh.listKeys());
        }
    },
    getKey: {
        help: 'getKey id [filename] - get public key by id [to file]',
        fn: function(args) {
            var parts = args.split(/\s+/);
            var id = parts.shift();
            var fn = false;
            if (parts.length) {
                fn = parts.join(' ');
            }
            var key = gh.getKey(id);
            console.dir(key);
            if (fn) {
                fs.writeFile(fn, key.key);
                console.log('>>> written to ' + fn);
            }
        }
    },
    createKey: {
        help: 'createKey name filename - create a key from file containing RSA/DSA key.',
        fn: function(args) {
            var parts = args.split(/\s+/);
            var name = parts.shift();
            var filename = parts.join(' ');
            var key = fs.readFile(filename);
            if (!key) {
                console.log('*** error: ' + filename + ' not found');
                return;
            }
            console.dir(gh.createKey(name, key));
        }
    },
    deleteKey: {
        help: 'deleteKey id - delete key identifeid by id.',
        fn: function(args) {
            console.dir(gh.deleteKey(args));
        }
    },
    listRepos: {
        help: 'listRepos [user] - list [a user\'s] repositories.',
        fn: function(args) {
            console.dir(gh.listRepos(args));
        }
    },
    listCollaborators: {
        help: 'listCollaborators user/repo - list a repository\'s collaborators.',
        fn: function(args) {
            console.dir(gh.listCollaborators(args));
        }
    },
    cd: {
        help: 'cd path - change working directory to path.',
        fn: function(args) {
            if (fs.chdir(args)) {
                console.log('*** error: ' + fs.error());
            }
            else {
                console.log('local directroy now ' + fs.getcwd());
            }
        }
    },
    '!': {
        help: '!cmd - run cmd as a shell command.',
        fn: function(cmd) {
            console.log(process.exec(cmd));
        }
    },
    quit: {
        help: 'quit - exit the program.',
        fn: function() {
            alive = false;
        }
    },
    help: {
        help: 'help - show this list.',
        fn: function() {
            commands.each(function(cmd) {
                console.log(cmd.help);
            });
        }
    }
}

function main(username, password) {
    if (!username) {
        console.log('Usage: GitHub.js username [password]');
        return;
    }
    if (!password) {
        password = console.getPassword('GitHub password for ' + username + ': ');
    }
    
    gh = new GitHub(username, password);
    stdin = new ReadLine('GitHub', 50);
    defaultPrompt();
    var line;
    while (alive) {
        try {
            line = stdin.gets();
        }
        catch (e) {
            console.log('');
            continue;
        }
        if (line === false) {
            break;
        }
        line = line.trim();
        if (line[0] === '!') {
            console.log(process.exec(line.substr(1).trim()));
            continue;
        }
        var parts = line.split(/\s+/);
        var cmd = parts.shift();
        var args = parts.join(' ');
        if (commands[cmd]) {
            commands[cmd].fn(args);
        }
        else {
            console.log('*** Invalid command.  Type help for command list.');
        }
    }
    console.log('');
    
//    console.dir(gh.status);
//    console.dir(gh.user);
//    console.dir(gh.deleteEmails('mykesx@gmail.com'));
//    console.dir(gh.getEmails());
}