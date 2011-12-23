#include "SilkJS.h"

using namespace v8;

Persistent<ObjectTemplate> globalObject;
extern Persistent<Context> context;
extern char *readFile(const char *s);

static Handle<Value> Log(const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
	printf("%ld %s\n", (unsigned long)getpid(), *str);
    return Undefined();
}

static Handle<Value> Print(const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
	printf("%s", *str);
    return Undefined();
}

static Handle<Value> Println(const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
	printf("%s\n", *str);
    return Undefined();
}

struct SCRIPTNODE {
	SCRIPTNODE *next;
	const char *name;
	time_t mtime;
	Persistent<Script> script;
};

SCRIPTNODE *scriptCache = NULL;

static Handle<Value> Include(const Arguments& args) {
	HandleScope scope;
	for (int i = 0; i < args.Length(); i++) {
		String::Utf8Value str(args[i]);
		char buf[strlen(*str) + 18 + 1];
		char *js_file = readFile(*str);
		if (!js_file) {
			strcpy(buf, *str);
			if (buf[0] != '/') {
				strcpy(buf, "/usr/share/silkjs/");
				strcat(buf, *str);
				js_file = readFile(buf);
			}
		}
		if (!js_file) {
			return ThrowException(String::Concat(String::New("include file not found "), args[i]->ToString()));
		}
		Handle<String> source = String::New(js_file);
		delete [] js_file;

		Handle<Script>script = Script::New(source, args[i]);;
		return script->Run();
	}
	return Undefined();
}

extern void init_buffer_object();
extern void init_console_object();
extern void init_logfile_object();
extern void init_process_object();
extern void init_sem_object();
extern void init_net_object();
extern void init_http_object();
extern void init_fs_object();
extern void init_mysql_object();
extern void init_sqlite3_object();
extern void init_gd_object();
extern void init_ncurses_object();
extern void init_v8_object();

void init_global_object() {
	HandleScope scope;
	globalObject = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
	
	init_buffer_object();
	init_console_object();
	init_logfile_object();
	init_process_object();
	init_sem_object();
	init_net_object();
	init_http_object();
	init_fs_object();
	init_mysql_object();
	init_sqlite3_object();
	init_gd_object();
	init_ncurses_object();
	init_v8_object();
	
	globalObject->Set(String::New("log"), FunctionTemplate::New(Log));
	globalObject->Set(String::New("print"), FunctionTemplate::New(Print));
	globalObject->Set(String::New("println"), FunctionTemplate::New(Println));
	globalObject->Set(String::New("include"), FunctionTemplate::New(Include));
}
