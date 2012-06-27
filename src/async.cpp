#include "SilkJS.h"

static JSVAL async_alloc_fd_set(JSARGS args) {
    fd_set *set = new fd_set;
    FD_ZERO(set);
    return External::New(set);
}

static JSVAL async_free_fd_set(JSARGS args) {
    fd_set *set = (fd_set *)JSEXTERN(args[0]);
    delete set;
    return Undefined();
}

static JSVAL async_fd_zero(JSARGS args) {
    fd_set *set = (fd_set *)JSEXTERN(args[0]);
    FD_ZERO(set);
    return args[0];
}

static JSVAL async_fd_set(JSARGS args) {
    int fd = args[0]->IntegerValue();
    fd_set *set = (fd_set *)JSEXTERN(args[1]);
    FD_SET(fd, set);
    return args[0];
}

static JSVAL async_fd_clr(JSARGS args) {
    int fd = args[0]->IntegerValue();
    fd_set *set = (fd_set *)JSEXTERN(args[1]);
    FD_CLR(fd, set);
    return args[0];
}

static JSVAL async_fd_isset(JSARGS args) {
    int fd = args[0]->IntegerValue();
    fd_set *set = (fd_set *)JSEXTERN(args[1]);
    return FD_ISSET(fd, set) ? True() : False();
}

static JSVAL async_select(JSARGS args) {
    fd_set  readfds,
            writefds,
            exceptfds,
            *fds;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int maxfd = args[0]->IntegerValue();
    if (!args[1]->IsNull() && !args[1]->IsUndefined()) {
        fds = (fd_set *)JSEXTERN(args[1]);
        readfds = *fds;
    }
    else {
        FD_ZERO(&readfds);
    }
    if (!args[2]->IsNull() && !args[2]->IsUndefined()) {
        fds = (fd_set *)JSEXTERN(args[2]);
        writefds = *fds;
    }
    else {
        FD_ZERO(&writefds);
    }
    if (!args[3]->IsNull() && !args[3]->IsUndefined()) {
        fds = (fd_set *)JSEXTERN(args[3]);
        exceptfds = *fds;
    }
    else {
        FD_ZERO(&exceptfds);
    }
    int ret = select(maxfd, &readfds, &writefds, &exceptfds, NULL);
    switch (ret) {
        case 0:
            return False();
        case -1:
            return False();
    }
    JSARRAY ra = Array::New();
    JSARRAY wa = Array::New();
    JSARRAY ea = Array::New();
    int randx = 0,
        wandx = 0,
        eandx = 0;
    for (int i=0; i<= maxfd; i++) {
        if (FD_ISSET(i, &readfds)) {
            ra->Set(randx++, Integer::New(i));
        }
        else if (FD_ISSET(i, &writefds)) {
            wa->Set(wandx++, Integer::New(i));
        }
        else if (FD_ISSET(i, &exceptfds)) {
            ea->Set(eandx++, Integer::New(i));
        }
    }
    JSOBJ o = Object::New();
    o->Set(String::New("read"), ra);
    o->Set(String::New("write"), wa);
    o->Set(String::New("except"), ea);
    return o;
}

JSVAL async_write(JSARGS args) {
    int fd = args[0]->IntegerValue();
    String::AsciiValue s(args[1]);
    size_t size = args[2]->IntegerValue();
    return Integer::New(write(fd, *s, size));
}

JSVAL async_read(JSARGS args) {
    int fd = args[0]->IntegerValue();
    size_t size = args[1]->IntegerValue();
    char *buf = new char[size];
    int ret = read(fd, buf, size);
    if (ret == 0) {
        delete[] buf;
        return False();
    }
    else if (ret < 0) {
        delete[] buf;
        return False();
    }
    else {
        Handle<String>s = String::New(buf);
        delete[] buf;
        return s;
    }
}

void init_async_object () {
    Handle<ObjectTemplate>async = ObjectTemplate::New();
    async->Set(String::New("alloc_fd_set"), FunctionTemplate::New(async_alloc_fd_set));
    async->Set(String::New("free_fd_set"), FunctionTemplate::New(async_free_fd_set));
    async->Set(String::New("FD_ZERO"), FunctionTemplate::New(async_fd_zero));
    async->Set(String::New("FD_SET"), FunctionTemplate::New(async_fd_set));
    async->Set(String::New("FD_CLR"), FunctionTemplate::New(async_fd_clr));
    async->Set(String::New("FD_ISSET"), FunctionTemplate::New(async_fd_isset));
    async->Set(String::New("select"), FunctionTemplate::New(async_select));
    async->Set(String::New("write"), FunctionTemplate::New(async_write));
    async->Set(String::New("read"), FunctionTemplate::New(async_read));
    builtinObject->Set(String::New("async"), async);
}
