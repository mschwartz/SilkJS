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
		write(logFd, logBuffer, *length);
		*length = 0;
	}
}

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
	globalObject->Set(String::New("logfile"), logfile);
}
