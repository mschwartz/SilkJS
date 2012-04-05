/**
 * @namespace builtin/popen
 * 
 * ### Synopsis
 * 
 * Interface to popen()
 */

#include "SilkJS.h"

static JSVAL popen_open(JSARGS args) {
    HandleScope scope;
	String::Utf8Value path(args[0]->ToString());
	String::Utf8Value type(args[1]->ToString());
    FILE *fp = fopen(*path, *type);
    if (!fp) {
        return scope.Close(String::New(strerror(errno)));
    }
    return scope.Close(External::New(fp));
}

static JSVAL popen_gets(JSARGS args) {
    HandleScope scope;
    FILE *fp = (FILE *)JSEXTERN(args[0]);
    int size = 4096;
    if (args.Length() > 1) {
        size = args[1]->IntegerValue();
    }
    char *buf = new char[size];
    char *result = fgets(buf, size, fp);
    if (!result) {
        delete [] buf;
        return scope.Close(False());
    }
    Handle<String>s = String::New(buf);
    delete [] buf;
    return scope.Close(s);
}

static JSVAL popen_puts(JSARGS args) {
    HandleScope scope;
    FILE *fp = (FILE *)JSEXTERN(args[0]);
	String::Utf8Value s(args[1]->ToString());
    if (fputs(*s, fp) == EOF) {
            return scope.Close(False());
    }
    return scope.Close(True());
}

static JSVAL popen_close(JSARGS args) {
    HandleScope scope;
    FILE *fp = (FILE *)JSEXTERN(args[0]);
    pclose(fp);
    return Undefined();
}

void init_popen_object() {
	HandleScope scope;

	JSOBJT popenObject = ObjectTemplate::New();
    
    popenObject->Set(String::New("open"), FunctionTemplate::New(popen_open));
    popenObject->Set(String::New("gets"), FunctionTemplate::New(popen_gets));
    popenObject->Set(String::New("puts"), FunctionTemplate::New(popen_puts));
    popenObject->Set(String::New("close"), FunctionTemplate::New(popen_close));

    builtinObject->Set(String::New("popen"), popenObject);
}
