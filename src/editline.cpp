#include "SilkJS.h"
#include <histedit.h>

struct EHANDLE {
    EditLine *el;
    char *prompt;
};

static inline EHANDLE *HANDLE(Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    return (EHANDLE *)JSEXTERN(v);
}

static char *prompt(EditLine *el) {
    EHANDLE *e;
    el_get(el, EL_CLIENTDATA, &e);
    return e->prompt;
}

static JSVAL editline_init(JSARGS args) {
    HandleScope scope;
	String::Utf8Value prog(args[0]->ToString());
    EditLine *el = el_init(*prog, stdin, stdout, stderr);
    EHANDLE *handle = new EHANDLE;
    handle->el = el;
    handle->prompt = strdup("> ");
    el_set(el, EL_CLIENTDATA, handle);
    el_set(el, EL_PROMPT, prompt);
    return scope.Close(External::New(handle));
}

static JSVAL editline_end(JSARGS args) {
    HandleScope scope;
    EHANDLE *e = HANDLE(args[0]);
    el_end(e->el);
    delete [] e->prompt;
    return Undefined();
}

static JSVAL editline_gets(JSARGS args) {
    HandleScope scope;
    EHANDLE *e = HANDLE(args[0]);
    int count;
    const char *s = el_gets(e->el, &count);
    if (!s) {
        return scope.Close(False());
    }
    return scope.Close(String::New(s));
}

static JSVAL editline_prompt(JSARGS args) {
    HandleScope scope;
    EHANDLE *e = HANDLE(args[0]);
	String::Utf8Value prompt(args[1]->ToString());
    if (e->prompt) {
        delete [] e->prompt;
    }
    e->prompt = strdup(*prompt);
    return Undefined();
}

void init_editline_object() {
	HandleScope scope;

	JSOBJT editlineObject = ObjectTemplate::New();

    editlineObject->Set(String::New("init"), FunctionTemplate::New(editline_init));
    editlineObject->Set(String::New("end"), FunctionTemplate::New(editline_end));
    editlineObject->Set(String::New("gets"), FunctionTemplate::New(editline_gets));
    editlineObject->Set(String::New("prompt"), FunctionTemplate::New(editline_prompt));

    builtinObject->Set(String::New("editline"), editlineObject);
}
