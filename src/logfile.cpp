/**
 * @module builtin/logfile
 * 
 * ### Synopsis
 * SilkJS builtin logfile object.
 * 
 * ### Description
 * 
 * This is an implementation of a process-safe logfile.  
 * 
 * One or more processes may log messages to the log file, without one process's write being interrupted by the OS scheduler switching to another process mid-write.
 * 
 * The logfile implementation uses a block of shared memory.  When a process calls logfile.write(), the message it passes is appended to the block of shared memory.  This is done within a semaphore, so only one write can happen at a time, blocking others attempting to do so.
 * 
 * If an attempt is made to append a string to the shared memory block but there is not enough room in the block for the string, the block is flushed to disk and reset to empty state.
 * 
 * A logfile.flush() method is implemented so a background process can force the shared memory block to be flushed to disk periodically.
 * 
 * An alternative to this implementation is to flock() and append directly to the log file by each process.  There is a serious performance hit for doing so.
 * 
 * ### Usage
 * var logfile = require('builtin/logfile');
 * 
 * ### Notes
 * 
 * The log file is written to /tmp/silkjs.log.  This really should be configurable.  
 * 
 * ### See Also
 * The JavaScriptimplementation of the http server.
 */

#include "SilkJS.h"

#define LOGFILE_CHUNK_SIZE	(16384*2)
#ifdef __APPLE__
#define LOGFILE_MM_FILENAME "/tmp/logfile_mm"
#define LOGFILE_FN "/tmp/silkjs.log"
#else
#define LOGFILE_MM_FILENAME "/dev/shm/logfile_mm"
#define LOGFILE_FN "/tmp/silkjs.log"
#endif

static MM *mm = NULL;
static char *logBuffer;
static long *length;
static int logFd = 0;

static void open_logfile() {
	if (!logFd) {
		logFd = open(LOGFILE_FN, O_RDWR);
		if (logFd < 0) {
			perror("logfile_write/open");
			exit(1);
		}
	}
}

static void lock_logfile() {
	open_logfile();
	flock(logFd, LOCK_EX);
}

static void unlock_logfile() {
	flock(logFd, LOCK_UN);
}

static void flush_logfile() {
	if (*length) {
		lseek(logFd, 0, 2);
		long toWrite = *length;
		long offset = 0;
		while (toWrite > 0) {
			long written = write(logFd, &logBuffer[offset], toWrite);
			offset += written;
			toWrite -= written;
		}
		*length = 0;
	}
}

/**
 * @function logfile.init
 * 
 * ### Synopsis
 * 
 * logfile.init();
 * 
 * Initialize the log file.
 * 
 */
static JSVAL logfile_init(JSARGS args) {
	HandleScope scope;
	if (!mm) {
		mm = mm_create(LOGFILE_CHUNK_SIZE*2, LOGFILE_MM_FILENAME);
		if (!mm) {
			printf("init_log_object error %s\n", mm_error());
			exit(1);
		}
		length = (long *)mm_malloc(mm, sizeof(long));
		*length = 0;
		logBuffer = (char *)mm_malloc(mm, LOGFILE_CHUNK_SIZE);
		*logBuffer = '\0';
	}
	return Undefined();
}

/**
 * @function logfile.write
 * 
 * ### Synopsis
 * 
 * logfile.write(s);
 * 
 * Write a string to the log file.  
 * 
 * The string is appended to the shared memory block.  The memory block is first flushed to disk if there is not enough room for the string.
 * 
 * @param {string} s - string to write to the log file.
 * 
 */
static JSVAL logfile_write(JSARGS args) {
	HandleScope scope;
	String::AsciiValue buf(args[0]);
	int len;
	if (args.Length() > 1) {
		len = args[1]->IntegerValue();
	}
	else {
		len = strlen(*buf);
	}
	lock_logfile();
	if (*length + len >= LOGFILE_CHUNK_SIZE) {
		flush_logfile();
	}
	memcpy(&logBuffer[*length], *buf, len);
	*length += len;
	unlock_logfile();
	return Undefined();
}

/**
 * @function logfile.flush
 * 
 * ### Synopsis
 * 
 * logfile.flush();
 * 
 * Flush contents of shared memory block to the log file, reset memory block to "empty" state.
 */
static JSVAL logfile_flush(JSARGS args) {
	HandleScope scope;
	lock_logfile();
	flush_logfile();
	unlock_logfile();
	return Undefined();
}

void init_logfile_object() {
	HandleScope scope;
	int fd = open(LOGFILE_FN, O_WRONLY|O_CREAT|O_TRUNC, 0664);
	close(fd);
	Handle<ObjectTemplate>logfile = ObjectTemplate::New();
	logfile->Set(String::New("init"), FunctionTemplate::New(logfile_init));
	logfile->Set(String::New("write"), FunctionTemplate::New(logfile_write));
	logfile->Set(String::New("flush"), FunctionTemplate::New(logfile_flush));
	builtinObject->Set(String::New("logfile"), logfile);
}
