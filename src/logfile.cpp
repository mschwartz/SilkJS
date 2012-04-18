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

struct STATE {
    bool alive;
    char *filename;
    char *mm_file;
    MM *mm;
    char *logBuffer;
    long *length;
    int logFd;

    STATE(char *filename) {
        this->alive = false;
        this->filename = strdup(filename);
        this->mm_file = new char[strlen(this->filename) + 4];
        strcpy(this->mm_file, filename);
        strcat(this->mm_file, "_mm");
        this->mm = mm_create(LOGFILE_CHUNK_SIZE * 2, this->mm_file);
        this->length = (long *) mm_malloc(mm, sizeof (long));
        *this->length = 0;
        this->logBuffer = (char *) mm_malloc(mm, LOGFILE_CHUNK_SIZE);
        *this->logBuffer = '\0';
        this->logFd = 0;
        this->alive = true;
        int fd = open(this->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        close(fd);
    }

    ~STATE() {
        if (this->logFd) {
            close(this->logFd);
        }
        this->logFd = 0;
        delete [] this->mm_file;
        delete [] this->filename;
        mm_destroy(this->mm);
    }
};

static inline STATE* HANDLE (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    STATE *state = (STATE *) JSEXTERN(v);
    return state;
}

/*
 * PRIVATE
 */

static void open_logfile (STATE *state) {
    if (!state->logFd) {
        state->logFd = open(state->filename, O_RDWR);
        if (state->logFd < 0) {
            perror("logfile_write/open");
            exit(1);
        }
    }
}

static void lock_logfile (STATE *state) {
    open_logfile(state);
    flock(state->logFd, LOCK_EX);
}

static void unlock_logfile (STATE *state) {
    flock(state->logFd, LOCK_UN);
}

static void flush_logfile (STATE *state) {
    if (*state->length) {
        lseek(state->logFd, 0, 2);
        long toWrite = *state->length;
        long offset = 0;
        while (toWrite > 0) {
            long written = write(state->logFd, &state->logBuffer[offset], toWrite);
            offset += written;
            toWrite -= written;
        }
        *state->length = 0;
    }
}

/**
 * @function logfile.init
 * 
 * ### Synopsis
 * 
 * var handle = logfile.init(filename);
 * 
 * Initialize the log file.
 * 
 * @param {string} filename - path to logfile
 * @returns {object} handle - handle to logfile
 */
static JSVAL logfile_init (JSARGS args) {
    HandleScope scope;
    String::AsciiValue filename(args[0]);
    STATE *state = new STATE(*filename);
    if (!state->alive) {
        ThrowException(String::New("Could not initialize log file"));
    }
    return scope.Close(External::New(state));
}

/**
 * @function logfile.write
 * 
 * ### Synopsis
 * 
 * logfile.write(handle, s);
 * logfile.write(handle, s, len);
 * 
 * Write a string to the log file.  
 * 
 * The string is appended to the shared memory block.  The memory block is first flushed to disk if there is not enough room for the string.
 * 
 * @param {object} handle - handle of the log file.
 * @param {string} s - string to write to the log file.
 * @param {int} len - optional length of string to write; defaults to strlen(s).
 * 
 */
static JSVAL logfile_write (JSARGS args) {
    HandleScope scope;
    STATE *state = HANDLE(args[0]);
    String::AsciiValue buf(args[1]);
    int len;
    if (args.Length() > 2) {
        len = args[2]->IntegerValue();
    }
    else {
        len = strlen(*buf);
    }
    lock_logfile(state);
    if (*state->length + len >= LOGFILE_CHUNK_SIZE) {
        flush_logfile(state);
    }
    memcpy(&state->logBuffer[*state->length], *buf, len);
    *state->length += len;
    unlock_logfile(state);
    return Undefined();
}

/**
 * @function logfile.flush
 * 
 * ### Synopsis
 * 
 * logfile.flush(handle);
 * 
 * Flush contents of shared memory block to the log file, reset memory block to "empty" state.
 * 
 * @param {object} handle - handle of logfile to flush.
 */
static JSVAL logfile_flush (JSARGS args) {
    HandleScope scope;
    STATE *state = HANDLE(args[0]);

    lock_logfile(state);
    flush_logfile(state);
    unlock_logfile(state);
    return Undefined();
}

/**
 * @function logfile.destroy
 * 
 * ### Synopsis
 * 
 * logfile.destroy(handle);
 * 
 * Destroy reference to a logfile, free all its resources.
 * 
 * @param {object} handle - handle to logfile to destroy.
 */
static JSVAL logfile_destroy (JSARGS args) {
    HandleScope scope;
    STATE *state = HANDLE(args[0]);
    delete state;
    return Undefined();
}

void init_logfile_object () {
    HandleScope scope;
    Handle<ObjectTemplate>logfile = ObjectTemplate::New();

    logfile->Set(String::New("init"), FunctionTemplate::New(logfile_init));
    logfile->Set(String::New("write"), FunctionTemplate::New(logfile_write));
    logfile->Set(String::New("flush"), FunctionTemplate::New(logfile_flush));
    logfile->Set(String::New("destroy"), FunctionTemplate::New(logfile_destroy));

    builtinObject->Set(String::New("logfile"), logfile);
}
