# About SilkJS

SilkJS is a console application that wraps the v8 JavaScript engine.  It is designed for Ubuntu Linux, but it also compiles for OSX as well as other flavors of Linux.

SilkJS implements a thin layer of "glue" to access the various operating system functions and functions of other useful libraries.

A great deal of focus on the SilkJS project is performance.  SilkJS was designed to facilitate the implementation of the included HTTP server, but has proven 
to be far more useful in numerous console and server-side applications.  The HTTP server has remarkable performance characteristics, and the core logic is written
entirely in JavaScript!

Some people say SilkJS is a synchronous version of NodeJS.  It was not designed to be such, but it was designed to be a faster alternative to NodeJS for 
Linux-based server applications.  

## Features

* CommonJS require 1.1 implementation
* XMLHttpRequest implementation for accessing WWW services
* Basic implementation of console as you typically would call in browser code.
* Rich set of POSIX File System methods.
* Rich set of Process methods.
* Implementation of glue for libgd2 for graphics manipulation.
* Implementation of HTTP "helper" methods (see below).
* Implementation of MySQL functions.
* Implementation of SQLite3 functions.
* Implementation of NCURSES functions.
* Implementation of NETWORK functions (sockets, etc.).
* Implementation of SSH2 functions (exec remote commands, etc.).
* Implementation of functions to manipulate the V8 engine itself.
* and more!

## License

SilkJS is intended to be free for use by anyone and for any legal purpose.

To that end, please see this link:

http://www.opensource.org/licenses/alphabetical

Pick whichever license you choose, as long as it is OSI approved.


## QUICKSTART

(See https://github.com/mschwartz/SilkJS/wiki/Build-Instructions for dependent libraries)

Currently for Mac OSX Lion x64 and Ubuntu only...

    git clone https://github.com/mschwartz/SilkJS SilkJS
    cd SilkJS
    make

Once built:

    ./silkjs httpd/main.js

Then point your browser at http://localhost:9090/

## Useful Links

Home Site: http://silkjs.org/

Google Group: http://groups.google.com/group/SilkJS

Check out the Wiki here, too: https://github.com/mschwartz/SilkJS/wiki

