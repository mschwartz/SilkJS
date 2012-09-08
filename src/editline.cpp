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

// https://github.com/antirez/linenoise
#include "linenoise.h"

static JSVAL editline_gets (JSARGS args) {
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
        if (addHistory && strlen(line)) {
            linenoiseHistoryAdd(line);
        }
        Local<Value> s = String::New(line);
        delete [] line;
        return s;
    }
    else {
        if (errno == EAGAIN) {
            return Integer::New(-4);
        }
        else if (errno == EPIPE) {
            return Integer::New(-2);
        }
        return False();
    }
}

static JSVAL editline_loadhistory (JSARGS args) {
    String::Utf8Value filename(args[0]->ToString());
    linenoiseHistoryLoad(*filename);
    return Undefined();
}

static JSVAL editline_savehistory (JSARGS args) {
    String::Utf8Value filename(args[0]->ToString());
    linenoiseHistorySave(*filename);
    return Undefined();
}

void init_editline_object () {
    JSOBJT editlineObject = ObjectTemplate::New();

    editlineObject->Set(String::New("gets"), FunctionTemplate::New(editline_gets));
    editlineObject->Set(String::New("loadHistory"), FunctionTemplate::New(editline_loadhistory));
    editlineObject->Set(String::New("saveHistory"), FunctionTemplate::New(editline_savehistory));

    builtinObject->Set(String::New("editline"), editlineObject);
}
