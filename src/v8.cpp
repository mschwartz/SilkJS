/**
 * @module builtin/v8
 * 
 * ### Synopsis
 * SilkJS builtin v8 object.
 * 
 * ### Description
 * The builtin/v8 object provides low-level access to some v8 JavaScript engine methods.
 * 
 * ### Usage
 * var net = require('builtin/v8');
 * 
 * ### See Also:
 * Google's v8 documentation and the source code
 */
#include "SilkJS.h"
#include <v8-debug.h>

struct ScriptWrapper {
    Persistent<Script> script;
};

/**
 * @function v8.gc
 * 
 * ### Synopsis
 * 
 * v8.gc();
 * 
 * Call v8's garbage collector.
 */
static JSVAL gc (const Arguments& args) {
    for (int i=0; i<10000; i++) {
        if (V8::IdleNotification()) {
            break;
        }
    }
    return Undefined();
}

/**
 * @function v8.compileScript
 * 
 * ### Synopsis
 * var script v8.compileScript(source);
 * 
 * Compile a JavaScript source (string) to a v8::Script object.  The returned script object should be treated as opaque by JavaScript code.
 * 
 * @param {string} source - JavaScript source code to compile.
 * @return {object} script - Compiled JavaScript (opaque handle)
 * 
 * ### Notes
 * You should free any scripts you compile with v8.freeScript().
 */

static JSVAL compileScript (JSARGS args) {
    //	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
    //	Context::Scope context_scope(context);
    ScriptWrapper *wrapper = new ScriptWrapper;
    wrapper->script = Persistent<Script>::New(Script::New(args[0]->ToString(), args[1]->ToString()));
    //	context.Dispose();
    return Opaque::New(wrapper);
}


/**
 * @function v8.runScript
 * 
 * ### Synopsis
 * 
 * var ret = v8.runScript(script);
 * 
 * Run a previously compiled v8::Script - see v8.compileScript above.
 * @param {object} script - opaque handle to previously compiled v8::Script.
 * @return whatever the script returns.
 */
// runScript(context, script)

static JSVAL runScript (JSARGS args) {
    ScriptWrapper *wrapper = (ScriptWrapper *) JSOPAQUE(args[0]); // wrap->Value();

    //	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
    //	Context::Scope context_scope(context);
    Handle<Value>v = wrapper->script->Run();
    //	context.Dispose();
    return v;
}

/**
 * @function v8.freeScript
 * 
 * ### Synopsis
 * 
 * v8.freeScript(script);
 * 
 * Free a previously compiled v8::Script.  Each compiled script consumes some amount of memory until it is freed.
 * 
 * @param {object} script - opaque handle to script to be freed.
 */
static JSVAL freeScript (JSARGS args) {
    ScriptWrapper *wrapper = (ScriptWrapper *) JSOPAQUE(args[0]);
    wrapper->script.Dispose();
    delete wrapper;
    return Undefined();
}

static void debugger () {
    extern Persistent<Context> context;
    Context::Scope scope(context);
    Debug::ProcessDebugMessages();
}

/**
 * @function v8.enableDebugger
 * 
 * ### Synopsis
 * 
 * This function enables the internal v8 debugger on port 5858.
 */
static JSVAL enableDebugger (JSARGS args) {
    HandleScope scope;
    Debug::SetDebugMessageDispatchHandler(debugger, true);
    Debug::EnableAgent("silkjs", 5858, true);
    Debug::DebugBreak();
    return Undefined();
}

void init_v8_object () {
    HandleScope scope;

    Handle<ObjectTemplate>v8 = ObjectTemplate::New();
    v8->Set(String::New("gc"), FunctionTemplate::New(gc));
    v8->Set(String::New("compileScript"), FunctionTemplate::New(compileScript));
    v8->Set(String::New("runScript"), FunctionTemplate::New(runScript));
    v8->Set(String::New("freeScript"), FunctionTemplate::New(freeScript));
    v8->Set(String::New("enableDebugger"), FunctionTemplate::New(enableDebugger));

    builtinObject->Set(String::New("v8"), v8);
}

