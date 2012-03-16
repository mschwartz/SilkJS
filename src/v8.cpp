#include "SilkJS.h"
#include <v8-debug.h>

struct ScriptWrapper {
	Persistent<Script> script;
};

static JSVAL gc(const Arguments& args) {
	HandleScope scope;
	while (!V8::IdleNotification()) {}
	return Undefined();
}

static JSVAL compileScript(JSARGS args) {
	HandleScope scope;

//	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
//	Context::Scope context_scope(context);
	ScriptWrapper *wrapper = new ScriptWrapper;
	wrapper->script = Persistent<Script>::New(Script::New(args[0]->ToString(), args[1]->ToString()));
//	context.Dispose();
	return scope.Close(External::New(wrapper));
}

// runScript(context, script)
static JSVAL runScript(JSARGS args) {
	HandleScope scope;
	
	Local<External>wrap = Local<External>::Cast(args[0]);
	ScriptWrapper *wrapper = (ScriptWrapper *)wrap->Value();

//	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
//	Context::Scope context_scope(context);
	Handle<Value>v = wrapper->script->Run();
//	context.Dispose();
	JSVAL ret = scope.Close(v);
	return ret;
}

static JSVAL freeScript(JSARGS args) {
	HandleScope scope;
	
	Local<External>wrap = Local<External>::Cast(args[0]);
	ScriptWrapper *wrapper = (ScriptWrapper *)wrap->Value();
	wrapper->script.Dispose();
	delete wrapper;
	return Undefined();
}

static void debugger() {
	extern Persistent<Context> context;
    Context::Scope scope(context);
    Debug::ProcessDebugMessages();
}


static JSVAL enableDebugger(JSARGS args) {
	HandleScope scope;
	Debug::SetDebugMessageDispatchHandler(debugger, true);
	Debug::EnableAgent("silkjs", 5858);
	return Undefined();
}


void init_v8_object() {
	HandleScope scope;

	Handle<ObjectTemplate>v8 = ObjectTemplate::New();
	v8->Set(String::New("gc"), FunctionTemplate::New(gc));
	v8->Set(String::New("compileScript"), FunctionTemplate::New(compileScript));
	v8->Set(String::New("runScript"), FunctionTemplate::New(runScript));
	v8->Set(String::New("freeScript"), FunctionTemplate::New(freeScript));
	v8->Set(String::New("enableDebugger"), FunctionTemplate::New(enableDebugger));

	builtinObject->Set(String::New("v8"), v8);
}

