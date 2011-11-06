#include "SilkJS.h"

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

	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
	Context::Scope context_scope(context);
	ScriptWrapper *wrapper = new ScriptWrapper;
	wrapper->script = Persistent<Script>::New(Script::New(args[0]->ToString(), args[1]->ToString()));
	context.Dispose();
	return scope.Close(External::New(wrapper));
}

// runScript(context, script)
static JSVAL runScript(JSARGS args) {
	HandleScope scope;
	
	Local<External>wrap = Local<External>::Cast(args[0]);
	ScriptWrapper *wrapper = (ScriptWrapper *)wrap->Value();

	Persistent<Context>context = Context::New(NULL, ObjectTemplate::New());
	Context::Scope context_scope(context);
	Handle<Value>v = wrapper->script->Run();
	context.Dispose();
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

void init_v8_object() {
	HandleScope scope;

	Handle<ObjectTemplate>v8 = ObjectTemplate::New();
	v8->Set(String::New("gc"), FunctionTemplate::New(gc));
	v8->Set(String::New("compileScript"), FunctionTemplate::New(compileScript));
	v8->Set(String::New("runScript"), FunctionTemplate::New(runScript));
	v8->Set(String::New("freeScript"), FunctionTemplate::New(freeScript));

	globalObject->Set(String::New("v8"), v8);
}

