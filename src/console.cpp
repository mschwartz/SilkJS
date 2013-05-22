/**
 * @module builtin/console
 * 
 * ### Synopsis
 * SilkJS builtin console object.
 * 
 * ### Description
 * The builtin/console object provides a minimal implementation of the client-side console object.
 * 
 * ### Usage
 * var console = require('builtin/console');
 * 
 * ### See Also
 * The JavaScriptimplementation of a more robust console object:
 * modules/console.js
 */
#include "SilkJS.h"
#include <sys/ioctl.h>

/**
 * @function console.log
 * 
 * ### Synopsis
 * 
 * console.log(s);
 * 
 * Write a string to stdout.
 * 
 * @param {string} s - the string to write to stdout
 */
static JSVAL log (JSARGS args) {
    String::AsciiValue s(args[0]);
    fprintf(stdout, "%s\n", *s);
    return Undefined();
}

/**
 * @function console.error
 * 
 * ### Synopsis
 * 
 * console.error(s);
 * 
 * Write a string to stderr.
 * 
 * @param {string} s - the string to write to stderr
 */
static JSVAL error (JSARGS args) {
    String::AsciiValue s(args[0]);
    fprintf(stderr, "%s\n", *s);
    return Undefined();
}

/**
 * @function console.getPassword
 * 
 * ### Synopsis
 * 
 * var password = console.getPassword(prompt);
 * 
 * Display a prompt and readin a password without echoing the characters to the display.
 * 
 * @param {string} prompt - prompt on the line to get the password
 * @returns {string} password - the password entered by the user
 */
static JSVAL getPassword (JSARGS args) {
    String::Utf8Value prompt(args[0]);
    return String::New(getpass(*prompt));
}

/**
 * @function console.getSize
 *
 * ### Synopsis
 *
 * var size = console.getSize();
 *
 * Get size of console window in rows and columns
 *
 * The size object returned contains the following members:
 * rows: number of rows the terminal can display
 * columns: number of columns the terminal can display
 *
 * @returns {object} size - object containing dimensions, as described above.
 */
static JSVAL getSize(JSARGS args) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    Handle<Object>o = Object::New();
    o->Set(String::New("Rows"), Integer::New(w.ws_row));
    o->Set(String::New("Columns"), Integer::New(w.ws_col));
    return o;
}

void init_console_object () {
    Handle<ObjectTemplate>console = ObjectTemplate::New();
    console->Set(String::New("log"), FunctionTemplate::New(log));
    console->Set(String::New("error"), FunctionTemplate::New(error));
    console->Set(String::New("getPassword"), FunctionTemplate::New(getPassword));
    console->Set(String::New("getSize"), FunctionTemplate::New(getSize));

    builtinObject->Set(String::New("console"), console);
}

