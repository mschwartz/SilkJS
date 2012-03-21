/** 
 * @namespace global
 * 
 * ### Synopsis
 * 
 * Methods initially available in the global object/namespace.
 * 
 * ### Description
 * 
 * The core SilkJS executable provides these methods in the global scope.  Typically, they will be overloaded by other modules or JavaScript files being included or reuqired.
 * 
 * The global object also contains a global.builtin namespace, which consists of the other C++/native builtin bindings.
 * 
 */
#include "SilkJS.h"

using namespace v8;

Persistent<ObjectTemplate> globalObject;
Persistent<ObjectTemplate> builtinObject;
extern Persistent<Context> context;
extern char *readFile(const char *s);

/**
 * @function global.log
 * 
 * ### Synopsis
 * 
 * log(s);
 * 
 * Write a string to stdout.
 * 
 * The string is prefixed with the current process's pid.
 * 
 * @param {string} s - the string to write to stdout.
 */
static Handle<Value> Log(const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
    printf("%ld %s\n", (unsigned long) getpid(), *str);
    return Undefined();
}

/**
 * @function global.print
 * 
 * ### Synopsis
 * 
 * print(s);
 * 
 * Write a string to stdout.
 * 
 * The string is NOT prefixed with the current process's pid, as is done by the log() function.
 * 
 * @param {string} s - the string to write to stdout.
 */
static Handle<Value> Print(const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
    printf("%s", *str);
    return Undefined();
}

/**
 * @function global.println
 * 
 * ### Synopsis
 * 
 * println(s);
 * 
 * Write a string to stdout, adding a newline to the end.
 * 
 * The string is NOT prefixed with the current process's pid, as is done by the log() function.
 * 
 * @param {string} s - the string to write to stdout.
 */
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

/**
 * @function global.include
 * 
 * ### Synopsis
 * 
 * include(path [,path...]);
 * 
 * Load, compile, and run the specified JavaScript files in the SilkJS context of the current process.
 * 
 * @param {string} path - the path in the file system to a file to include.
 */
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
        ScriptOrigin origin(String::New(*str), Integer::New(0), Integer::New(0));
        Handle<Script>script = Script::New(source, &origin);
        script->Run();
    }
    return Undefined();
}

extern void init_buffer_object();
extern void init_console_object();
extern void init_process_object();
extern void init_v8_object();
extern void init_net_object();
extern void init_fs_object();
extern void init_http_object();
#if !BOOTSTRAP_SILKJS
extern void init_sem_object();
extern void init_mysql_object();
extern void init_sqlite3_object();
extern void init_gd_object();
extern void init_ncurses_object();
extern void init_logfile_object();
extern void init_curl_object();
extern void init_xhrHelper_object();
extern void init_ssh_object();
#endif

void init_global_object() {
    HandleScope scope;
    globalObject = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
    builtinObject = Persistent<ObjectTemplate>::New(ObjectTemplate::New());

    init_buffer_object();
    init_console_object();
    init_process_object();
    init_net_object();
    init_http_object();
    init_fs_object();
    init_v8_object();

#if !BOOTSTRAP_SILKJS
    init_logfile_object();
    init_sem_object();
    init_mysql_object();
    init_sqlite3_object();
    init_gd_object();
    init_ncurses_object();
    init_curl_object();
    init_xhrHelper_object();
    init_ssh_object();
#endif
    globalObject->Set(String::New("builtin"), builtinObject);
    globalObject->Set(String::New("log"), FunctionTemplate::New(Log));
    globalObject->Set(String::New("print"), FunctionTemplate::New(Print));
    globalObject->Set(String::New("println"), FunctionTemplate::New(Println));
    globalObject->Set(String::New("include"), FunctionTemplate::New(Include));
}
