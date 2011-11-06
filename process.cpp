#include "SilkJS.h"

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

static JSVAL process_getpid(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(getpid()));
}

static JSVAL process_fork(JSARGS args) {
	HandleScope scope;
	pid_t pid = fork();
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

void init_process_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>process = ObjectTemplate::New();
	process->Set(String::New("error"), FunctionTemplate::New(process_error));
	process->Set(String::New("getpid"), FunctionTemplate::New(process_getpid));
	process->Set(String::New("fork"), FunctionTemplate::New(process_fork));
	process->Set(String::New("exit"), FunctionTemplate::New(process_exit));
	process->Set(String::New("sleep"), FunctionTemplate::New(process_sleep));
	process->Set(String::New("wait"), FunctionTemplate::New(process_wait));
	process->Set(String::New("exec"), FunctionTemplate::New(process_exec));
	
	globalObject->Set(String::New("process"), process);
}
