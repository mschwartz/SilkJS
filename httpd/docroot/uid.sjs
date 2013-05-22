var process = require('builtin/process'),
    Util = require('Util');

res.contentType = 'text/plain';

res.writeln(Util.print_r(Config));
res.writeln('getuid = ' + process.getuid());
res.writeln('getgid = ' + process.getgid());
