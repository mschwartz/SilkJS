/**
 * @module builtin/watchdog
 *
 * ### Synopsis
 * SilkJS builtin watchdog object
 *
 * ### Description
 * SIGALRM handler controllable by JavaScript
 * The idea is to allow JavaScript code to set the alarm, run it's code
 * and if the JavaScript code takes too long, the SIGALRM handler is
 * invoked and the current process exits.
 *
 * Specifically used in the HTTP server to prevent runaway processes.
 *
 * ### Usage
 * var watchdog = require('builtin/watchdog');
 */

#include "SilkJS.h"

static pid_t inttostr(char *s, pid_t n) {
	 unsigned int i = 1000000000;

	 if( ((signed)n) < 0 ) {
		 *s++ = '-';
		 n = -n;
	 }

	 while( i > n ) i /= 10;

	 do {
		 *s++ = '0' + (n-n%i)/i%10;
	 } while( i /= 10 );

	 *s = 0;

	 return n;
}

static void watchdogHandler(int sig) {
	pid_t pid = getpid();
	char buf[128] = "Process ";
	inttostr(&buf[8], pid);
	strcat(buf, " exited due to watchdog\n");
	write(STDOUT_FILENO, buf, strlen(buf));
	exit(0);
}

/**
 * @function watchdog.set
 *
 * ### Synopsis
 * 
 * watchdog.set(seconds);
 *
 * Sets the watchdog timer to count down for the specified time.  When the watchdog timer 
 * elapses, the process will exit.
 *
 * Most likely, you will want to clear the timer or call set() again to keep the timer from
 * killing the process.
 *
 * @param {int} seconds - number of seconds to count down
 */
static JSVAL watchdog_set(JSARGS args) {
	static int installed = 0;
	if (!installed) {
		signal(SIGALRM, watchdogHandler);
		installed = 1;
	}
	alarm(args[0]->IntegerValue());
	return Undefined();
}

/**
 * @function  watchdog.clear
 *
 * ### Synopsis
 *
 * watchdog.clear();
 *
 * Clears the watchdog timer so the watchdog will not kill the current process.
 */
static JSVAL watchdog_clear(JSARGS args) {
	alarm(0);
	return Undefined();
}

void init_watchdog_object() {
	JSOBJT watchdogObject = ObjectTemplate::New();
	watchdogObject->Set(String::New("set"), FunctionTemplate::New(watchdog_set));
	watchdogObject->Set(String::New("clear"), FunctionTemplate::New(watchdog_clear));
	builtinObject->Set(String::New("watchdog"), watchdogObject);
}

