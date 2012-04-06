/**
 * @module builtin/editline
 * 
 * ### Synopsis
 * 
 * var editline = require('builtin/editline');
 * 
 * Interface to libedit (line editor/readline substitute)
 */
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

/**
 * @function editline.init
 * 
 * ### Synopsis
 * 
 * var handle = editline.init(programName);
 * 
 * Initialize editline.  The programName is used to deal with .editrc.  See man editline for details.
 * 
 * @param (string) programName - name of program.
 * @return {object} handle - opaque handle to use with other editline methods.
 */
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

/**
 * @function editline.end
 * 
 * ### Synopsis
 * 
 * editline.end(handle);
 * 
 * End editline session for the given handle, freeing any resources used.
 * 
 * @param {object} handle - handle returned from editline.init();
 */
static JSVAL editline_end(JSARGS args) {
    HandleScope scope;
    EHANDLE *e = HANDLE(args[0]);
    el_end(e->el);
    delete [] e->prompt;
    return Undefined();
}

/**
 * @function editline.gets
 * 
 * ### Synopsis
 * 
 * var line = editline.gets(handle);
 * 
 * Read a line from the console using libedit.
 * 
 * @param {object} handle - handle returned from editline.init();
 * @return {string} line - line read from console or false on EOF or error.
 */
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

/**
 * @function editline.prompt
 * 
 * ### Synopsis
 * 
 * editline.prompt(handle, prompt);
 * 
 * Set the prompt for lines read with libedit.
 * 
 * If you set the prompt to "foo> ", then the user will see "foo> " (without the quotes) and the cursor placed after that.
 * 
 * @param {object} handle - handle returned from editline.init();
 * @param {string) prompt - the prompt to set
 */
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
