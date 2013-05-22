#!/usr/local/bin/silkjs

var process = require('builtin/process'),
    fs = require('fs'),
    console = require('console');


var upstartTemplate = [
    'description "SilkJS WWW Server"',
    'author      "Mike Schwartz - http://silkjs.net"',
    '',
    '# Assure all mounts are ready when booting',
    'start on started mountall',
    'stop on shutdown',
    '',
    '# Automatically Respawn:',
    'respawn',
    'respawn limit 99 5',
    '',
    'script',
    '   # Not sure why $HOME is needed...',
    '   export HOME="/root"',
    '',
    '   cd {serverdir}',
    '   exec /usr/local/bin/httpd-silk.js {bootstrap} >> /var/log/httpd-silk-stdout.log 2>&1',
    'end script',
    '',
    'post-start script',
    '   # Optionally put a script here that will notifiy you that SilkJS has (re)started',
    'end script'
].join('\n');

function gets(prompt) {
    var ret = builtin.editline.gets(prompt, false);
    if (ret === false || ret < 0) {
        console.log('\n*** Aborted');
        process.exit(0);
    }
    return ret;
}

function fail(msg) {
    console.log('*** ' + msg);
    process.exit(1);
}

function main() {
    var uid = process.getuid();
    if (process.env().OS !== 'LINUX') {
        fail('This script works on linux only.');
    }
    console.log([
        '',
        'This script generates an upstart script to automatically',
        'start your SilkJS WWW server on boot and allow you to:',
        '   % start silkjs',
        '   % stop silkjs',
        '   % initctl status silkjs',
        'etc.'
    ].join('\n'));
    if (!fs.exists('/etc/init') || !fs.isDir('/etc/init')) {
        fail('This script requires a system with upstart installed.');
    }
    if (uid !== 0) {
        fail('This script must be run as root, or use sudo.');
    }

    console.log([
        '',
        'Server Directory is the directory where your SilkJS WWW project',
        'is found.  Typically, the documentRoot is a subdirectory of',
        'the Server Directory.'
    ].join('\n'));
    var serverDir;

    while (true) {
        serverDir = gets('Server Directory: ');

        if (serverDir[0] !== '/') {
            console.log('*** ' + serverDir + ' is not an absolute path (begin with /)');
            continue;
        }
        if (!fs.isDir(serverDir)) {
            console.log('*** ' + serverDir + ' does not exist');
            continue;
        }
        break;
    }
    serverDir = serverDir.replace(/\/$/, '');

    console.log([
        '',
        'Bootstrap is the name of the .js file that is passed on the',
        'command line to httpd-silk.js.'
    ].join('\n'));
    var bootstrap;
    while (true) {
        bootstrap = gets('Bootstrap filename: ');
        if (!fs.exists(serverDir + '/' + bootstrap)) {
            console.log('*** ' + serverDir + '/' + bootstrap + ' does not exist');
            continue;
        }
        break;
    }

    if (!fs.exists(serverDir + '/' + bootstrap)) {
        fail(serverDir + '/' + bootstrap + ' does not exist');
    }
    fs.writeFile('/etc/init/silkjs.conf', upstartTemplate.replace(/{serverDir}/igm, serverDir).replace(/{bootstrap}/igm, bootstrap));

    console.log('file written to /etc/init/silkjs.conf');
    console.log('  sudo start silkjs');
    console.log('to start the server');
}
