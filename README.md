# About SilkJS

SilkJS is a console application that wraps the v8 JavaScript engine.  It is designed for Ubuntu Linux, but it also compiles for OSX as well as other flavors of Linux.

SilkJS implements a thin layer of "glue" to access the various operating system functions and functions of other useful libraries.

A great deal of focus on the SilkJS project is performance.  SilkJS was designed to facilitate the implementation of the included HTTP server, but has proven 
to be far more useful in numerous console and server-side applications.  The HTTP server has remarkable performance characteristics, and the core logic is written
entirely in JavaScript!

Some people say SilkJS is a synchronous version of NodeJS.  It was not designed to be such, but it was designed to be a faster alternative to NodeJS for 
Linux-based server applications.  You might think of it as the command-line version of JavaScript, akin to command-line versions of PHP or Perl.  Like in those languages,
you can implement alternatives to shell scripts, command-line applications, and even full blown Internet servers.

## CoffeeScript

SilkJS HTTPD is a full featured CoffeeScript server.  For more details, see:

https://github.com/mschwartz/SilkJS/wiki/CoffeeScript

## Graphical Debugger

SilkJS supports the V8 debugger.  You can use node-inspector to debug your programs.

For more details, see:
https://github.com/mschwartz/SilkJS/wiki/Debugging

## Features

Perhaps the greatest feature of SilkJS is that you write synchronous code; there are no callback function implementations required on your part.  The theory of
operation is exactly the opposite of NodeJS - that synchronous programming is more natural, and the code actually runs faster if you let the OS do its thing.  There 
is no "nested callback hell" with SilkJS.  Not only is your code more readable (top-to-bottom, left-to-right), various sample programs we've converted from NodeJS to
SilkJS are quite a few lines shorter.  On top of this, your synchronous code makes sense in both server and command line applications (why do you need to do async
I/O in a command-line program?).

SilkJS runs CoffeeScript pages, Showdown/Markdown pages, and JSP-like JST JavaScript pages.  If you
have a file in your documentRoot with a .coffee extension, it is loaded, compiled, cached, and run
upon request.  If the file is changed on disk, it is automatically reloaded, compiled, cached, and 
run.  The same is true for JST programs, files ending with .jst extension.  Files ending with .md
extension are loaded and interpreted each time using Showdown (Markdown processor).  If you have
a file named index.coffee, index.jst, or index.md, that file is run for URIs for that directory that end with /.


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
* JSP-like JavaScript templates
* Execute CoffeeScript programs server-side to generate HTML
* Showdown/Markdown templates
* and more!

## License

SilkJS is intended to be free for use by anyone and for any legal purpose.

To that end, please see this link:

http://www.opensource.org/licenses/alphabetical

Pick whichever license you choose, as long as it is OSI approved.

## HTTP Server

The SilkJS HTTP server is fast... REALLY fast.  It's also robust enough to handle 20,000 concurrent requests (as tested with ab).

On a I7-2630QM CPU @ 2.00GHz, SilkJS HTTP outperforms lighthttpd and nodejs (v0.6.5):

```
REQUESTS PER SECOND

silkjs = 26759 rps
lighttpd = 17183 rps (v1.4.29)
nodejs = 2212 (v0.6.5 - single core, simple static server)
```

The HTTP server requires a tiny bit of code that is not in JavaScript.  The reading of mulit-part MIME POST data containing binary data
cannot be done in JavaScript because JavaScript does not natively support a binary data type.  Otherwise, the server is written almost entirely
in JavaScript.  Even the CommonJS require() method is written in JavaScript.  

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

Home Site: http://silkjs.net/ (POWERED BY SILKJS, not Apache, not ngnix)

Google Group: http://groups.google.com/group/SilkJS

Check out the Wiki here, too: https://github.com/mschwartz/SilkJS/wiki

