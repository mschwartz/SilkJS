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
    HandleScope scope;
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
    HandleScope scope;
    String::AsciiValue s(args[0]);
    fprintf(stderr, "%s\n", *s);
    return Undefined();
}

/**
 * @function console.getPassword
 * 
 * var password = console.getPassword(prompt);
 * 
 * ### Synopsis
 * 
 * Display a prompt and readin a password without echoing the characters to the display.
 * 
 * @param {string} prompt - prompt on the line to get the password
 * @returns {string} password - the password entered by the user
 */
static JSVAL getPassword (JSARGS args) {
    HandleScope scope;
    String::Utf8Value prompt(args[0]);
    return scope.Close(String::New(getpass(*prompt)));
}

void init_console_object () {
    HandleScope scope;

    Handle<ObjectTemplate>console = ObjectTemplate::New();
    console->Set(String::New("log"), FunctionTemplate::New(log));
    console->Set(String::New("error"), FunctionTemplate::New(error));
    console->Set(String::New("getPassword"), FunctionTemplate::New(getPassword));

    builtinObject->Set(String::New("console"), console);
}

