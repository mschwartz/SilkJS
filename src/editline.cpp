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
#include <setjmp.h>

#define USE_LINENOISE
#ifdef USE_LINENOISE
// https://github.com/antirez/linenoise
#include "linenoise.h"

//static sig_atomic_t gotsig = 0;
//static void sig(int i) {
//    printf("GOT SIGNAL %d\n", i);
//    signal(i, sig);
//    gotsig = i;
//}

static bool initialized = false;
static JSVAL editline_gets(JSARGS args) {
    HandleScope scope;
    bool addHistory = true;
    addHistory = args[1]->BooleanValue();
//    if (!initialized) {
//        printf("initializing\n");
//        initialized = true;
//        (void) signal(SIGINT, sig);
//        (void) signal(SIGQUIT, sig);
//        (void) signal(SIGHUP, sig);
//        (void) signal(SIGTERM, sig);
//    }
    String::Utf8Value prompt(args[0]->ToString());
    char *line = linenoise(*prompt);
    if (line) {
        if (addHistory) {
            linenoiseHistoryAdd(line);
        }
        Local<Value> s = String::New(line);
        delete [] line;
        return scope.Close(s);
    }
    else {
        if (errno == EAGAIN) {
            return scope.Close(Integer::New(-4));
        }
        else if (errno == EPIPE) {
            return scope.Close(Integer::New(-2));
        }
        return scope.Close(False());
    }
}

static JSVAL editline_loadhistory(JSARGS args) {
    HandleScope scope;
    String::Utf8Value filename(args[0]->ToString());
    linenoiseHistoryLoad(*filename);
    return Undefined();
}

static JSVAL editline_savehistory(JSARGS args) {
    HandleScope scope;
    String::Utf8Value filename(args[0]->ToString());
    linenoiseHistorySave(*filename);
    return Undefined();
}

void init_editline_object() {
	HandleScope scope;

	JSOBJT editlineObject = ObjectTemplate::New();

    editlineObject->Set(String::New("gets"), FunctionTemplate::New(editline_gets));
    editlineObject->Set(String::New("loadHistory"), FunctionTemplate::New(editline_loadhistory));
    editlineObject->Set(String::New("saveHistory"), FunctionTemplate::New(editline_savehistory));

    builtinObject->Set(String::New("editline"), editlineObject);
}

#else
#include <histedit.h>

struct EHANDLE {
    EditLine *el;
    History *h;
    char *prompt;
    jmp_buf jmpbuf;
};

static EHANDLE *currentHandle = NULL;

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

static sig_atomic_t gotsig = 0;

static void sig(int i) {
    signal(i, sig);
    gotsig = i;
    siglongjmp(currentHandle->jmpbuf, 1);
}

/**
 * @function editline.init
 * 
 * ### Synopsis
 * 
 * var handle = editline.init(programName);
 * var handle = editline.init(programName, historySize);
 * 
 * Initialize editline.  The programName is used to deal with .editrc.  See man editline for details.
 * 
 * @param (string) programName - name of program.
 * @param (int) historySize - size of history (number of lines of history to keep), defaults to 50.
 * @return {object} handle - opaque handle to use with other editline methods.
 */
static JSVAL editline_init(JSARGS args) {
    HandleScope scope;
	String::Utf8Value prog(args[0]->ToString());
    int historySize = 50;
    if (args.Length() > 1) {
        historySize = args[1]->IntegerValue();
    }
    EditLine *el = el_init(*prog, stdin, stdout, stderr);
    EHANDLE *handle = new EHANDLE;
    handle->el = el;
    handle->prompt = strdup("> ");
    el_set(el, EL_SIGNAL, 1);
    el_set(el, EL_CLIENTDATA, handle);
    el_set(el, EL_PROMPT, prompt);
    el_set(el, EL_EDITOR, "vi");
    handle->h = history_init();
    HistEvent ev;
    history(handle->h, &ev, H_SETSIZE, historySize);
    history(handle->h, &ev, H_SETUNIQUE, 1);
    el_set(el, EL_HIST, history, handle->h);
    (void) signal(SIGINT, sig);
    (void) signal(SIGQUIT, sig);
    (void) signal(SIGHUP, sig);
    (void) signal(SIGTERM, sig);

    el_source(el, NULL);
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
    history_end(e->h);
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
 * var line = editline.gets(handle, addHistory);
 *
 * Read a line from the console using libedit.
 * 
 * @param {object} handle - handle returned from editline.init();
 * @param {boolean} addHistory - true to add input line to history.  Defaults to true.
 * @return {string} line - line read from console or false on EOF or error.
 */
static JSVAL editline_gets(JSARGS args) {
//    HandleScope scope;
    EHANDLE *e = HANDLE(args[0]);
    int count;
    
    if (sigsetjmp(e->jmpbuf, 1) != 0) {
        el_reset(e->el);
        if (gotsig) {
            int retval = 0;
            switch (gotsig) {
                case SIGINT:
                    retval = -1;
                    break;
                case SIGQUIT:
                    retval = -2;
                    break;
                case SIGHUP:
                    retval = -3;
                    break;
                case SIGTERM:
                    retval = -4;
                    break;
            }
            gotsig = 0;
            return Integer::New(retval);
        }
        else {
            return False();
        }
    }
    currentHandle = e;
    const char *s = el_gets(e->el, &count);
    if (!s) {
        return False();
    }
    HistEvent ev;
    history(e->h, &ev, H_ENTER, s);
    return String::New(s);
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
#endif
