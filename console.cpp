// see http://invisible-island.net/ncurses/man/ncurses.3x.html

#include "SilkJS.h"

static JSVAL log(JSARGS args) {
	HandleScope scope;
	String::AsciiValue s(args[0]);
	fprintf(stdout, "%s\n", *s);
	return Undefined();
}

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
	
	globalObject->Set(String::New("console"), console);
}

