/**
 * @module builtin/process
 * 
 * ### Synopsis
 * SilkJS builtin process object.
 * 
 * ### Description
 * The builtin/process object provides constants and methods to directly access the underlying operating system's process-oriented functions.  
 * 
 * ### Usage
 * var process = require('builtin/process');
 * 
 * ### See Also
 * Operating system man pages
 * 
 */
#include "SilkJS.h"
//#include "v8-read-only/src/v8.h"
#include <pwd.h>
#include <grp.h>
#ifdef __APPLE__
#include <uuid/uuid.h>
#endif

// Ubuntu 13.x doesn't include resource.h proper
#ifndef RUSAGE_SELF
#include <linux/rusage.h>
#endif

// TODO:
// getcwd()
// chdir()
// signals
// atexit, on_exit
// popen/exec/etc.

/**
 * @function process.error
 * 
 * ### Synopsis
 * Returns string version of last OS error.
 * 
 * ### Usage:
 * var message = process.error();
 * 
 * @return {string} message - error message.
 */
static JSVAL process_error (JSARGS args) {
    return String::New(strerror(errno));
}

/**
 * @function process.getuid
 *
 * ### Synopsis
 * Returns real user ID (uid) of calling process
 *
 * ### Usage:
 * var uid = process.getuid();
 *
 * @return {int} uid - real user ID
 */
static JSVAL process_getuid(JSARGS args) {
    return Integer::New(getuid());
}

/**
 * @function process.setuid
 *
 * ### Synopsis
 * Set real user ID (uid) of calling process
 *
 * ### Usage:
 * var status = process.getuid(newId);
 *
 * @param {int} newId - user ID to set for calling process
 * @return {int} status - 0 if successful, -1 if not (call process.error for details)
 */
static JSVAL process_setuid(JSARGS args) {
    return Integer::New(setuid(args[0]->IntegerValue()));
}

/**
 * @function process.getgid
 *
 * ### Synopsis
 * Returns real group ID (gid) of calling process
 *
 * ### Usage:
 * var gid = process.getgid();
 *
 * @return {int} gid - real group ID
 */
static JSVAL process_getgid(JSARGS args) {
    return Integer::New(getgid());
}

/**
 * @function process.setgid
 *
 * ### Synopsis
 * Set real group ID (gid) of calling process
 *
 * ### Usage:
 * var status = process.getgid(newId);
 *
 * @param {int} newId - group ID to set for calling process
 * @return {int} status - 0 if successful, -1 if not (call process.error for details)
 */
static JSVAL process_setgid(JSARGS args) {
    return Integer::New(setgid(args[0]->IntegerValue()));
}

static JSVAL format_passwd(struct passwd *p) {
    if (!p) {
        return False();
    }
    JSOBJ o = Object::New();
    o->Set(String::New("name"), String::New(p->pw_name));
    o->Set(String::New("passwd"), String::New(p->pw_passwd));
    o->Set(String::New("uid"), Integer::New(p->pw_uid));
    o->Set(String::New("gid"), Integer::New(p->pw_gid));
    o->Set(String::New("gecos"), String::New(p->pw_gecos));
    o->Set(String::New("dir"), String::New(p->pw_dir));
    o->Set(String::New("shell"), String::New(p->pw_shell));
    return o;
}

/**
 * @function process.getpwnam
 *
 * ### Synopsis
 * Get user information from the system user database
 *
 * ### Usage:
 * var info = process.getpwnam(login);
 *
 * ### Info Structure
 * The returned object has the following fields:
 * name: login name
 * passwd: the user's password (may not be reliable if using shadow, etc.)
 * uid: the user's user ID
 * gid: the user's group ID
 * gecos: "user information"
 * dir: user's home directory
 * shell: user's shell
 *
 * @param {string} login - login name of user to get info for
 * @return {int} info - object as described above (Info Structure)
 */
static JSVAL process_getpwnam(JSARGS args) {
    String::Utf8Value login(args[0]->ToString());
    return format_passwd(getpwnam(*login));
}

/**
 * @function process.getpwuid
 *
 * ### Synopsis
 * Get user information from the system user database
 *
 * ### Usage:
 * var info = process.getpwnam(uid);
 *
 * ### Info Structure
 * The returned object has the following fields:
 * name: login name
 * passwd: the user's password (may not be reliable if using shadow, etc.)
 * uid: the user's user ID
 * gid: the user's group ID
 * gecos: "user information"
 * dir: user's home directory
 * shell: user's shell
 *
 * @param {int} uid - user Id of user to get info for
 * @return {int} info - object as described above (Info Structure)
 */
static JSVAL process_getpwuid(JSARGS args) {
    return format_passwd(getpwuid(args[0]->IntegerValue()));
}

static JSVAL format_group(struct group *g) {
    if (!g) {
        return False();
    }
    JSOBJ o = Object::New();
    o->Set(String::New("name"), String::New(g->gr_name));
    o->Set(String::New("passwd"), String::New(g->gr_passwd));
    o->Set(String::New("gid"), Integer::New(g->gr_gid));

    Handle<Array>a = Array::New();
    int ndx = 0;
    char **mem = g->gr_mem;
    while (*mem) {
        a->Set(ndx++, String::New(*mem));
        mem++;
    }
    o->Set(String::New("mem"), a);
    return o;
}

/**
 * @function process.getgrnam
 *
 * ### Synopsis
 * Get group information from the system user database
 *
 * ### Usage:
 * var info = process.getgrnam(name);
 *
 * ### Info Structure
 * The returned object has the following fields:
 * name: group name
 * passwd: the group's password (may not be reliable if using shadow, etc.)
 * gid: the user's group ID
 * mem: array of member names (strings)
 *
 * @param {string} name - name of group to get info for
 * @return {int} info - object as described above (Info Structure)
 */
static JSVAL process_getgrnam(JSARGS args) {
    String::Utf8Value name(args[0]->ToString());
    return format_group(getgrnam(*name));
}

/**
 * @function process.getgruid
 *
 * ### Synopsis
 * Get group information from the system user database
 *
 * ### Usage:
 * var info = process.getgrgid(gid);
 *
 * ### Info Structure
 * The returned object has the following fields:
 * name: group name
 * passwd: the group's password (may not be reliable if using shadow, etc.)
 * gid: the user's group ID
 * mem: array of member names (strings)
 *
 * @param {int} gid - group ID of group to get info for
 * @return {int} info - object as described above (Info Structure)
 */
static JSVAL process_getgrgid(JSARGS args) {
    return format_group(getgrgid(args[0]->IntegerValue()));
}

/**
 * @function process.kill
 * 
 * ### Synopsis
 * 
 * var success = process.kill(pid);
 * 
 * Send the SIGKILL signal to the specified process (by pid).
 * 
 * @param {int} pid - process ID (pid) of process to kill.
 * @return {int} success - 0 on success, 1 if an error occurred.
 */
static JSVAL process_kill (JSARGS args) {
    pid_t pid = args[0]->IntegerValue();
    return Integer::New(kill(pid, SIGKILL));
}

/**
 * @function process.getpid
 * 
 * ### Synopsis
 * 
 * var my_pid = process.getpid();
 * 
 * Returns the pid of the current process.
 * 
 * @return {int} my_pid - process ID (pid) of the current process.
 */
static JSVAL process_getpid (JSARGS args) {
    return Integer::New(getpid());
}

/**
 * @function process.fork
 * 
 * ### Synopsis
 * 
 * var pid = process.fork();
 * 
 * Create a new process.
 * 
 * ### Description
 * 
 * Fork() causes creation of a new process.  The new process (child process) is an exact copy of the calling process (parent process) except for the following:
 * 
 * 1. The child process has a unique process ID.
 * 
 * 2. The child process has a different parent process ID (i.e., the process ID of the parent process).
 * 
 * 3. The child process has its own copy of the parent's descriptors. These descriptors reference the same underlying objects, so that, for instance, file pointers in file objects are shared between the child and the parent, so that an lseek(2) on a descriptor in the child process can affect a subsequent read or write by the parent.  This descriptor copying is also used by the shell to establish standard input and output for newly created processes as well as to set up pipes.
 * 
 * 4. The child processes resource utilizations are set to 0; see the man page for setrlimit(2).
 * 
 * @return 0 to the child, the pid of the created process to the parent.  If an error occurred, -1 is returned.
 */
static JSVAL process_fork (JSARGS args) {
    extern Persistent<Context> context;
    context->Exit();
    pid_t pid = fork();
    context->Enter();
    return Integer::New(pid);
}

/**
 * @function process.exit
 * 
 * ### Synopsis
 * 
 * process.exit(status);
 * 
 * Terminate the current process or program, returning status to the parent or shell.
 * 
 * @param {int} status - status code to return to parent or shell.
 * @return NEVER
 */
static JSVAL process_exit (JSARGS args) {
    exit(args[0]->IntegerValue());
    return Undefined();
}

/**
 * @function process.sleep
 * 
 * ### Synopsis
 * process.sleep(seconds);
 * 
 * Suspend execution of the current process for specified number of seconds.
 * 
 * @param {int} seconds - number of seconds to suspend.
 */
static JSVAL process_sleep (JSARGS args) {
    sleep(args[0]->IntegerValue());
    return Undefined();
}

/**
 * @function process.usleep
 * 
 * ### Synopsis
 * process.usleep(microseconds);
 * 
 * Suspend execution of the current process for specified number of microseconds.
 * 
 * @param {int} microseconds - number of microseconds to suspend.
 */
static JSVAL process_usleep (JSARGS args) {
    usleep(args[0]->IntegerValue());
    return Undefined();
}

/**
 * @function process.wait
 * 
 * ### Synopsis
 * 
 * var o = process.wait();
 * var o = process.wait(poll)
 * 
 * Wait for process termination.
 * 
 * ### Description
 * 
 * This function suspends execution of its calling process until one of its child processes terminates.  The function returns an object of the form:
 * 
 * + pid: the pid of the child process that terminated
 * + status: the status returned by the child process
 * 
 * @param {boolean} poll - if true, then this function will return immediately whether a child process has exited or not.
 * @return {object} o - information about the process that terminated.  If polling and no process exited, false is returned.
 * 
 * ### See Also
 * process.exit()
 */
static JSVAL process_wait (JSARGS args) {
    int status, 
        flags = 0;

    if (args.Length() > 0) {
        if (args[0]->BooleanValue()) {
            flags = WNOHANG;
        }
    }
    pid_t childPid = waitpid(-1, &status, flags);
    if (childPid == -1) {
        perror("wait ");
        return False();
    }
    if (childPid == 0) {
        return False();
    }
    // if (flags) {
    //     childPid = waitpid(childPid, &status, 0);
    //     if (childPid == -1) {
    //         perror("waitpid ");
    //     }
    // }
    Handle<Object>o = Object::New();
    o->Set(String::New("pid"), Integer::New(childPid));
    o->Set(String::New("status"), Integer::New(status));
    return o;
}

/**
 * @function process.exec
 * 
 * ### Synopsis
 * 
 * var output = process.exec(command_line);
 * 
 * Execute a Unix command, returning the output of the command (it's stdout) as a JavaScript string.
 * 
 * ### Description
 * 
 * This function calls popen() with the specified command line and reads its output until end of file.  Under the hood, a fork() and exec() is performed which is not particularly fast.  Still it can be useful to execute shell commands.
 * 
 * @param {string} command_line - a Unix command to execute
 * @return {string} output - the output of the command executed.
 */
static int exec_result;
static JSVAL process_exec (JSARGS args) {
    String::AsciiValue cmd(args[0]);
    string s;
    char buf[2048];
    FILE *fp = popen(*cmd, "r");
    int fd = fileno(fp);
    while (ssize_t size = read(fd, buf, 2048)) {
        s.append(buf, size);
    }
    exec_result = pclose(fp);
    return String::New(s.c_str(), s.size());
}

/**
 * @function process.exec_result();
 *
 * ### Synopsis
 *
 * var result = process.exec_result);
 *
 * Returns the exit code of the last process.exec() call.  
 *
 * NOTE: The value is the low-order 8 bits of the argument the program run by exec()  passed to _exit(2) or exit(3).
 *
 * @return {int} result - exit code
 */
static JSVAL process_exec_result(JSARGS args) {
    return Integer::New(WEXITSTATUS(exec_result));
}

/**
 * @function process.env
 * 
 * ### Synopsis
 * 
 * var env = process.env();
 * 
 * Get a hash of key/value pairs representing the environment of the calling process.
 * 
 * Typical kinds of environment variables you'll see returned by this function are:
 * HOME - user's home directory.
 * PATH - the sequence of directories, separated by colons, searched for command execution.
 * ...
 * 
 * @return {object} env - hash of key/value environment variables.
 */
static JSVAL process_env (JSARGS args) {
    extern char **environ;
    int size = 0;
    while (environ[size]) size++;
    Handle<Object>env = Object::New();
    char *home = NULL;
    for (int i = 0; i < size; ++i) {
        const char* key = environ[i];
        const char* val = strchr(key, '=');
        const int klen = val ? val - key : strlen(key);
        if (val[0] == '=') val = val + 1;
        const int vlen = val ? strlen(val) : 0;
        env->Set(String::New(key, klen), String::New(val, vlen));
        if (!strcmp(key, "HOME")) {
            home = (char *) val;
        }
    }
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        home = pw->pw_dir;
        env->Set(String::New("HOME"), String::New(home));
    }
    // set number of CPU cores
    env->Set(String::New("CORES"), Integer::New(sysconf(_SC_NPROCESSORS_ONLN)));
    // OS TYPE
#ifdef __APPLE__
    env->Set(String::New("OS"), String::New("OSX"));
#elif __linux__
    env->Set(String::New("OS"), String::New("LINUX"));
#else
    env->Set(String::New("OS"), String::New("UNIX"));
#endif
    // 32 bit or 64 bit OS
#ifdef __x86_64__
    env->Set(String::New("PROCESSOR"), String::New("64"));
#else
    env->Set(String::New("PROCESSOR"), String::New("32"));
#endif
    return env;
}

static JSVAL process_setenv(JSARGS args) {
    String::AsciiValue key(args[0]);
    String::AsciiValue value(args[1]);
	return Integer::New(setenv(*key, *value, 1));
}

/**
 * @function process.rusage
 * 
 * ### Synopsis
 * 
 * var o = process.rusage();
 * 
 * Get resource usage information for current process.
 * 
 * The object returned is of the form:
 * 
 * + time: total user + system CPU time used.
 * + utime: user CPU time used (float, in seconds).
 * + stime: system CPU time used (float, in seconds).
 * + maxrss: maximum resident set size.
 * + ixrss: integral shared memory size.
 * + idrss: integral unshared data size.
 * + isrss: integral unshared stack size.
 * + minflt: page reclaimes (soft page faults).
 * + majflt: page faults (hard page faults).
 * + nswap: swaps.
 * + inblock: block input operations.
 * + oublock: block output operations.
 * + msgsnd: IPC messages sent.
 * + msgrcv: IPC messages received.
 * + nsignals: signals received.
 * + nvcsw: voluntary context switches.
 * + nivcsw: involuntary context switches.
 */
static double timeval2sec (const timeval& t) {
    double f = (double) t.tv_sec + t.tv_usec / 1000000.0;
    f = long(f * 1000000.0 + .5);
    f /= 1000000;
    return f;
}

static timeval addTime (timeval& t1, timeval& t2) {
    timeval t;
    t.tv_sec = t1.tv_sec + t2.tv_sec;
    t.tv_usec = t1.tv_usec + t2.tv_usec;

    if (t.tv_usec >= 1000000) {
        t.tv_sec += 1;
        t.tv_usec -= 1000000;
    }

    return t;
}

static JSVAL process_rusage (JSARGS args) {
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
    JSOBJ o = Object::New();
    o->Set(String::New("time"), Number::New(timeval2sec(addTime(r.ru_utime, r.ru_stime))));
    o->Set(String::New("utime"), Number::New(timeval2sec(r.ru_utime)));
    o->Set(String::New("stime"), Number::New(timeval2sec(r.ru_stime)));
    o->Set(String::New("maxrss"), Integer::New(r.ru_maxrss));
    o->Set(String::New("ixrss"), Integer::New(r.ru_ixrss));
    o->Set(String::New("idrss"), Integer::New(r.ru_idrss));
    o->Set(String::New("isrss"), Integer::New(r.ru_isrss));
    o->Set(String::New("minflt"), Integer::New(r.ru_minflt));
    o->Set(String::New("majflt"), Integer::New(r.ru_majflt));
    o->Set(String::New("nswap"), Integer::New(r.ru_nswap));
    o->Set(String::New("inblock"), Integer::New(r.ru_inblock));
    o->Set(String::New("oublock"), Integer::New(r.ru_oublock));
    o->Set(String::New("msgend"), Integer::New(r.ru_msgsnd));
    o->Set(String::New("msgrcv"), Integer::New(r.ru_msgrcv));
    o->Set(String::New("nsignals"), Integer::New(r.ru_nsignals));
    o->Set(String::New("nvcsw"), Integer::New(r.ru_nvcsw));
    o->Set(String::New("nivcsw"), Integer::New(r.ru_nivcsw));
    return o;
}

/**
 * @function process.getlogin
 * 
 * ### Synopsis
 * 
 * var username = process.getlogin();
 * 
 * Get a string containing the name of the user logged in on the controlling terminal of the process.
 * @return {string} username - name of user or false if error.
 */
static JSVAL process_getlogin (JSARGS args) {
    char *s = getlogin();
    if (!s) {
        return False();
    }
    return String::New(s);
}

void init_process_object () {
    Handle<ObjectTemplate>process = ObjectTemplate::New();
    process->Set(String::New("env"), FunctionTemplate::New(process_env));
    process->Set(String::New("setenv"), FunctionTemplate::New(process_setenv));
    process->Set(String::New("error"), FunctionTemplate::New(process_error));
    
    process->Set(String::New("getuid"), FunctionTemplate::New(process_getuid));
    process->Set(String::New("setuid"), FunctionTemplate::New(process_setuid));
    process->Set(String::New("getgid"), FunctionTemplate::New(process_getgid));
    process->Set(String::New("setgid"), FunctionTemplate::New(process_setgid));
    process->Set(String::New("getpwnam"), FunctionTemplate::New(process_getpwnam));
    process->Set(String::New("getpwuid"), FunctionTemplate::New(process_getpwuid));
    process->Set(String::New("getgrnam"), FunctionTemplate::New(process_getgrnam));
    process->Set(String::New("getgrgid"), FunctionTemplate::New(process_getgrgid));

    process->Set(String::New("kill"), FunctionTemplate::New(process_kill));
    process->Set(String::New("getpid"), FunctionTemplate::New(process_getpid));
    process->Set(String::New("fork"), FunctionTemplate::New(process_fork));
    process->Set(String::New("exit"), FunctionTemplate::New(process_exit));
    process->Set(String::New("sleep"), FunctionTemplate::New(process_sleep));
    process->Set(String::New("usleep"), FunctionTemplate::New(process_usleep));
    process->Set(String::New("wait"), FunctionTemplate::New(process_wait));
    process->Set(String::New("exec"), FunctionTemplate::New(process_exec));
    process->Set(String::New("exec_result"), FunctionTemplate::New(process_exec_result));
    process->Set(String::New("rusage"), FunctionTemplate::New(process_rusage));
    process->Set(String::New("getlogin"), FunctionTemplate::New(process_getlogin));

    builtinObject->Set(String::New("process"), process);
}
