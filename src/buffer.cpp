#include "SilkJS.h"

static JSVAL buffer_create(JSARGS args) {
	HandleScope scope;
	Buffer *buf = new Buffer;
#ifndef BUFFER_STRING
	buf->mem = new unsigned char[16384];
	buf->mem[0] = '\0';
	buf->size = 16384;
	buf->pos = 0;
#endif
	return scope.Close(External::New(buf));
}

static JSVAL buffer_reset(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
#ifdef BUFFER_STRING
	buf->s.clear();
#else
	buf->pos = 0;
	buf->mem[0] = '\0';
#endif
	return Undefined();
}

static JSVAL buffer_destroy(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
#ifndef BUFFER_STRING
	delete[] buf->mem;
#endif
	delete buf;
	return Undefined();
}

static JSVAL buffer_write(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
    String::Utf8Value data(args[1]);
#ifdef BUFFER_STRING
	buf->s += *data;
#else
	long len = args[2]->IntegerValue();
    if (buf->pos + len >= buf->size) {
		while (buf->pos + len >= buf->size) {
			buf->size *= 2;
		}
		buf->mem = (unsigned char *)realloc(buf->mem, buf->size);
    }
    memcpy(&buf->mem[buf->pos], *data, len);
    buf->pos += len;
    buf->mem[buf->pos] = '\0';
#endif
	return Undefined();
}

static JSVAL buffer_write64(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
    String::Utf8Value data(args[1]);
	buf->s += Base64Decode(*data);
	return Undefined();
}

static JSVAL buffer_read(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
#ifdef BUFFER_STRING
	return scope.Close(String::New(buf->s.c_str(), buf->s.size()));
#else
	return scope.Close(String::New((char *)buf->mem, buf->pos));
#endif
}

static JSVAL buffer_size(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	Buffer *buf = (Buffer *)wrap->Value();
#ifdef BUFFER_STRING
	return scope.Close(Integer::New(buf->s.size()));
#else
	return scope.Close(Integer::New(buf->pos));
#endif
}

void init_buffer_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>buffer = ObjectTemplate::New();
	buffer->Set(String::New("create"), FunctionTemplate::New(buffer_create));
	buffer->Set(String::New("reset"), FunctionTemplate::New(buffer_reset));
	buffer->Set(String::New("destroy"), FunctionTemplate::New(buffer_destroy));
	buffer->Set(String::New("write"), FunctionTemplate::New(buffer_write));
	buffer->Set(String::New("write64"), FunctionTemplate::New(buffer_write64));
	buffer->Set(String::New("read"), FunctionTemplate::New(buffer_read));
	buffer->Set(String::New("size"), FunctionTemplate::New(buffer_size));

	builtinObject->Set(String::New("buffer"), buffer);
}
