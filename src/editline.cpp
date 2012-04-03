#include "SilkJS.h"
#include <histedit.h>

static inline EditLine *HANDLE(Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    return (EditLine *)JSEXTERN(v);
}

static JSVAL editline_init(JSARGS args) {
    HandleScope scope;
	String::Utf8Value prog(args[0]->ToString());
    EditLine *e = el_init(*prog, stdin, stdout, stderr);
    return scope.Close(External::New(e));
}

static JSVAL editline_end(JSARGS args) {
    HandleScope scope;
    EditLine *e = HANDLE(args[0]);
    el_end(e);
    return Undefined();
}

static JSVAL editline_gets(JSARGS args) {
    HandleScope scope;
    EditLine *e = HANDLE(args[0]);
    int count;
    const char *s = el_gets(e, &count);
    if (!s) {
        return scope.Close(False());
    }
    return scope.Close(String::New(s));
}

void init_editline_object() {
	HandleScope scope;

	JSOBJT editlineObject = ObjectTemplate::New();

    editlineObject->Set(String::New("init"), FunctionTemplate::New(editline_init));
    editlineObject->Set(String::New("end"), FunctionTemplate::New(editline_end));
    editlineObject->Set(String::New("gets"), FunctionTemplate::New(editline_gets));

    builtinObject->Set(String::New("editline"), editlineObject);
}
