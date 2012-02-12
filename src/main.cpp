/* 
 * File:   main.cpp
 * Author: mschwartz
 *
 * Created on July 2, 2011, 2:27 PM
 */

#include "SilkJS.h"
#include <v8-debug.h>
//#define DEBUGME
//#define NOFORK


char *readFile(const char *fn) {
	int fd = open(fn, O_RDONLY);
	if (fd < 0) {
		return NULL;
	}
	long size = lseek(fd, 0, 2);
	lseek(fd, 0, 0);
	char *file = new char[size+1];
	size = read(fd, file, size);
	file[size] = '\0';
	close(fd);
	return file;
}

void AnsiSignalHandler(int sig) {
    signal(sig, AnsiSignalHandler);
    printf("Caught Signal %d for process: %ld\n", sig, (size_t)getpid());
	exit(11);
}

//int serverSocket;

Persistent<Context> context;
Persistent<Script> mainScript;
Persistent<Function> mainFunc;

void debugger() {
	HandleScope scope;
	Locker lock;
	Debug::ProcessDebugMessages();
	Unlocker unlocker;
}

int main(int argc, char** argv) {
	signal(SIGSEGV, AnsiSignalHandler);
//    printf("SILK V0.1\n");
    if (argc < 2) {
		printf("usage: %s file.js\n", argv[0]);
		exit(1);
    }
	
	char *startup = readFile(argv[1]);
	if (!startup) {
		printf("%s not found\n", argv[1]);
		exit(1);
	}
	if (startup[0] == '#' && startup[1] == '!') {
		startup[0] = startup[1] = '/';
	}
	{
//		Isolate *isolate = Isolate::New();
//		isolate->Enter();
//		Locker lock(isolate);
		HandleScope scope;
		
		init_global_object();
		context = Context::New(NULL, globalObject);
		Context::Scope context_scope(context);
        V8::SetCaptureStackTraceForUncaughtExceptions(true);
//		Debug::EnableAgent("silkjs", 9222);
//		Debug::SetDebugMessageDispatchHandler(debugger, true);

		Handle<Script>init = Script::New(String::New("global=this; module = {};"), String::New("builtin"));
		init->Run();
		

		mainScript = Persistent<Script>::New(Script::Compile(String::New(startup), String::New(argv[1])));
		TryCatch tryCatch;
		Handle<Value>v = mainScript->Run();
		if (v.IsEmpty()) {
		    Handle<Message>message = tryCatch.Message();
//		    Handle<String>text = message->Get();
            String::AsciiValue exception(tryCatch.Exception());
            String::AsciiValue filename(message->GetScriptResourceName());
		    printf("%s in %s line %d\n", *exception, *filename, message->GetLineNumber());
		    exit(1);
		}
		Handle<String> process_name = String::New("main");
		Handle<Value> process_val = context->Global()->Get(process_name);
		if (!process_val.IsEmpty()) {
            Handle<Function> process_fun = Handle<Function>::Cast(process_val);
            mainFunc = Persistent<Function>::New(process_fun);
            const int ac = argc-2;
            Handle<Value>av[ac];
            for (int i=2; i<argc; i++) {
                av[i-2] = String::New(argv[i]);
            }
    //		printf("SILKJS running %s\n", argv[1]);
            v = mainFunc->Call(context->Global(), ac, av);
            if (v.IsEmpty()) {
                Handle<Message>message = tryCatch.Message();
    //		    Handle<String>text = message->Get();
                String::AsciiValue exception(tryCatch.Exception());
                String::AsciiValue filename(message->GetScriptResourceName());
                printf("%s in %s line %d\n", *exception, *filename, message->GetLineNumber());
            }
        }
	}
}

