#include "SilkJS.h"

static JSVAL time_gettimeofday(JSARGS args) {
	timeval t;
	gettimeofday(&t, NULL);
	double v = t.tv_sec + (t.tv_usec/1000000.0);
	return Number::New(v);
}

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
static JSVAL time_getrusage(JSARGS args) {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return Number::New(timeval2sec(addTime(r.ru_utime, r.ru_stime)));
}

void init_time_object () {
    Handle<ObjectTemplate>time = ObjectTemplate::New();
    time->Set(String::New("gettimeofday"), FunctionTemplate::New(time_gettimeofday));
    time->Set(String::New("getrusage"), FunctionTemplate::New(time_getrusage));
    builtinObject->Set(String::New("time"), time);
}
