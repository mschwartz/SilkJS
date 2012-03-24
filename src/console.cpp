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
static JSVAL log(JSARGS args) {
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
static JSVAL error(JSARGS args) {
	HandleScope scope;
	String::AsciiValue s(args[0]);
	fprintf(stderr, "%s\n", *s);
	return Undefined();
}

void init_console_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>console = ObjectTemplate::New();
	console->Set(String::New("log"), FunctionTemplate::New(log));
	console->Set(String::New("error"), FunctionTemplate::New(error));
	
	builtinObject->Set(String::New("console"), console);
}

