#include "SilkJS.h"
#include "v8-read-only/src/v8.h"
#include "v8-read-only/src/objects.h"
#include "v8-read-only/src/isolate.h"
// TODO:
// getcwd()
// chdir()
// signals
// atexit, on_exit
// popen/exec/etc.

static JSVAL process_error(JSARGS args) {
	HandleScope scope;
	return scope.Close(String::New(strerror(errno)));
}

static JSVAL process_kill(JSARGS args) {
	HandleScope scope;
	pid_t pid = args[0]->IntegerValue();
	return scope.Close(Integer::New(kill(pid, SIGKILL)));
}

static JSVAL process_getpid(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(getpid()));
}

static JSVAL process_fork(JSARGS args) {
	extern Persistent<Context> context;
//	Isolate *isolate = Isolate::GetCurrent();
	context->Exit();
//	isolate->Exit();
	pid_t pid = fork();
//	isolate->Enter();
	context->Enter();
	HandleScope scope;
	return scope.Close(Integer::New(pid));
}

static JSVAL process_exit(JSARGS args) {
	HandleScope scope;
	exit(args[0]->IntegerValue());
	return Undefined();
}

static JSVAL process_sleep(JSARGS args) {
	HandleScope scope;
	sleep(args[0]->IntegerValue());
	return Undefined();
}

static JSVAL process_usleep(JSARGS args) {
	HandleScope scope;
	usleep(args[0]->IntegerValue());
	return Undefined();
}

static JSVAL process_wait(JSARGS args) {
	HandleScope scope;
	int status;
	pid_t childPid = waitpid(-1, &status, 0);
	if (childPid == -1) {
		perror("wait ");
	}
	Handle<Object>o = Object::New();
	o->Set(String::New("pid"), Integer::New(childPid));
	o->Set(String::New("status"), Integer::New(status));
	return scope.Close(o);
}

static JSVAL process_exec(JSARGS args) {
	HandleScope scope;
	String::AsciiValue cmd(args[0]);
	string s;
	char buf[2048];
	FILE *fp = popen(*cmd, "r");
	int fd = fileno(fp);
	while (ssize_t size = read(fd, buf, 2048)) {
		s.append(buf, size);
	}
	pclose(fp);
	return scope.Close(String::New(s.c_str(), s.size()));
}

static JSVAL process_getuid(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(getuid()));
}

// 20111227 markc@renta.net MIT license
static JSVAL process_env(JSARGS args) {
	extern char **environ;
	HandleScope scope;
	int size = 0;
	while (environ[size]) size++;
	Handle<Object>env = Object::New();
	for (int i = 0; i < size; ++i) {
		const char* key = environ[i];
		const char* val = strchr(key, '=');
		const int klen = val ? val - key : strlen(key);
		if (val[0] == '=') val = val + 1;
		const int vlen = val ? strlen(val) : 0;
		env->Set(String::New(key, klen), String::New(val, vlen));
	}
	return scope.Close(env);
}

void init_process_object() {
	HandleScope scope;

	Handle<ObjectTemplate>process = ObjectTemplate::New();
	process->Set(String::New("env"), FunctionTemplate::New(process_env));
	process->Set(String::New("error"), FunctionTemplate::New(process_error));
	process->Set(String::New("kill"), FunctionTemplate::New(process_kill));
	process->Set(String::New("getpid"), FunctionTemplate::New(process_getpid));
	process->Set(String::New("fork"), FunctionTemplate::New(process_fork));
	process->Set(String::New("exit"), FunctionTemplate::New(process_exit));
	process->Set(String::New("sleep"), FunctionTemplate::New(process_sleep));
	process->Set(String::New("usleep"), FunctionTemplate::New(process_usleep));
	process->Set(String::New("wait"), FunctionTemplate::New(process_wait));
	process->Set(String::New("exec"), FunctionTemplate::New(process_exec));
	process->Set(String::New("getuid"), FunctionTemplate::New(process_getuid));

	builtinObject->Set(String::New("process"), process);
}
