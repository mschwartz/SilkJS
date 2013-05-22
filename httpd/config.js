// httpd/config.js
/*
 * This is the default Config object for the HTTP server.  You would override these values in your WWW App's bootstrap.js file.
 *
 * For example (bootstrap.js):
 *
 * Config.port = 8080; // listen on 8080 instead of 9090
 * Config.numChildren = 50; // start and maintain 50 HttpChild processes.
 * Or:
 *
 * Config.extend({
 *      port: 8080, // listen on 8080 instead of 9090
 *      numChildren: 50 // start and maintain 50 HttpChild processes
 * });
 */

(function() {
    var fs = require('builtin/fs'),
        process = require('builtin/process'),
        user = process.getpwnam('www-data') || process.getpwnam('_www') || process.getpwnam('nobody'),
        group = process.getgrnam('www-data') || process.getgrnam('_www') || process.getgrnam('nogroup') || process.getgrnam('nobody'),
        docRoot = 'httpd/docroot';

    if (!fs.exists(docRoot)) {
        docRoot = '/usr/local/silkjs/httpd/docroot';
    }
    if (!fs.exists(docRoot)) {
        docRoot = '/usr/share/silkjs/httpd/docroot';
    }
    include.path.unshift(docRoot);
    Config = {
        port: 9090,
        // user and group are ignored if server not run as root
        // e.g. will run as you while developing
        user: user.name,    // username to run child processes as
        group: group.name,  // groupname to run child processes as
        numChildren: 50,
        requestsPerChild: 100000,
        watchdogTimeout: 30,    // if process runs this long for a request, the alarm handler will exit()
        listenIp: '0.0.0.0',    // listen socket will be bound to this IP.  '0.0.0.0' means ANY IP on this machine.
        documentRoot: docRoot,
        logFile: '/tmp/httpd-silkjs.log',
        directoryIndex: [
            'index.sjs',
            'index.jst',
            'index.coffee',
            'index.md',
            'index.html'
        ],
        jstPath: '',    // can set this to where include() within JST pages searches for included files.
        // you can set up a default MySQL credentials/db here.  The HttpChild will automatically create a connection, if this is configured, as global SQL variable.
    //  mysql: {
    //      host: 'localhost',
    //      user: 'xxx',
    //      passwd: 'xxx',
    //      db: 'xxx'
    //  },
        lockFile: '/tmp/silkf.lock'
    };

}());
