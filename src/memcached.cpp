/**
 * @module builtin/memcached
 * 
 * ### Synopsis
 * 
 * var memcache = require('builtin/memcached');
 * 
 * Builtin interface to libmemcached.
 * 
 * ### Constants
 * This module exposes the following constants:
 * 
 * DEFAULT_PORT: the default port for connecting to the memcached servers
 * VERSION: the libmemcached version string
 */
#include "SilkJS.h"
#include <libmemcached/memcached.h>

#define M memcached_st
#define R memcached_return_t
#define S memcached_server_st

static inline M* HANDLE(Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    return (M *)JSEXTERN(v);
}

JSVAL _memcached_connect(JSARGS args) {
	HandleScope scope;
	String::Utf8Value options(args[0]);
	M *handle = memcached_create(NULL);
	S *servers = memcached_servers_parse(*options);
	if (memcached_server_push(handle, servers)) {
		memcached_free(handle);
		return scope.Close(False());
	}
	return scope.Close(External::New(handle));
}

JSVAL _memcached_close(JSARGS args) {
	HandleScope scope;
	M* handle = HANDLE(args[0]);
	memcached_free(handle);
	return Undefined();
}

JSVAL _memcached_error(JSARGS args) {
	HandleScope scope;
	M* handle = HANDLE(args[0]);
	R rc = (R)args[0]->IntegerValue();
	return scope.Close(String::New(memcached_strerror(handle, rc)));
}

JSVAL _memcached_get(JSARGS args) {
	HandleScope scope;
	M* handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	size_t value_length;
	uint32_t flags;
	R rc;
	char *res = memcached_get(handle, *key, strlen(*key), &value_length, &flags, &rc);
	if (!res) {
		return scope.Close(False());
	}
	JSOBJ o = Object::New();
	o->Set(String::New("value"), String::New(res));
	o->Set(String::New("flags"), Integer::New(flags));
	o->Set(String::New("rc"), Integer::New(rc));
	free(res);
	return scope.Close(o);
}

JSVAL _memcached_mget(JSARGS args) {
	HandleScope scope;
	M* handle = HANDLE(args[0]);
	Handle<Array> aKeys = Handle<Array>::Cast(args[1]);
	int numKeys = aKeys->Length();
	char *keys[numKeys];
	size_t key_lengths[numKeys];
	for (int i=0; i<numKeys; i++) {
		String::Utf8Value k(aKeys->Get(i));
		keys[i] = *k;
		key_lengths[i] = strlen(keys[i]);
	}
	R rc = memcached_mget(handle, keys, key_lengths, numKeys);
	if (rc != MEMCACHED_SUCCESS) {
		return String::New(memcached_strerror(handle, rc));
	}
	char return_key[MEMCACHED_MAX_KEY];
	size_t return_key_length;
	char *return_value;
	size_t return_value_length;
	uint32_t flags;
	JSOBJ result = Object::New();
	while ((return_value = memcached_fetch(handle, return_key, &return_key_length, &return_value_length, &flags, &rc))) {
		JSOBJ o = Object::New();
		o->Set(String::New("value"), String::New(return_value));
		o->Set(String::New("flags"), Integer::New(flags));
		o->Set(String::New("rc"), Integer::New(rc));
		free(return_value);
		result->Set(String::New(return_key), o);
	}
	return scope.Close(result);
}

JSVAL _memcached_set(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	String::Utf8Value value(args[2]);
	time_t expiration = 0;
	if (args.Length() > 3) {
		expiration = args[3]->IntegerValue();
	}
	uint32_t flags = 0;
	if (args.Length() > 4) {
		flags = args[4]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_set(handle, *key, strlen(*key), *value, strlen(*value), expiration, flags)));
}

JSVAL _memcached_add(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	String::Utf8Value value(args[2]);
	time_t expiration = 0;
	if (args.Length() > 3) {
		expiration = args[3]->IntegerValue();
	}
	uint32_t flags = 0;
	if (args.Length() > 4) {
		flags = args[4]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_add(handle, *key, strlen(*key), *value, strlen(*value), expiration, flags)));
}

JSVAL _memcached_replace(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	String::Utf8Value value(args[2]);
	time_t expiration = 0;
	if (args.Length() > 3) {
		expiration = args[3]->IntegerValue();
	}
	uint32_t flags = 0;
	if (args.Length() > 4) {
		flags = args[4]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_replace(handle, *key, strlen(*key), *value, strlen(*value), expiration, flags)));
}

JSVAL _memcached_prepend(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	String::Utf8Value value(args[2]);
	time_t expiration = 0;
	if (args.Length() > 3) {
		expiration = args[3]->IntegerValue();
	}
	uint32_t flags = 0;
	if (args.Length() > 4) {
		flags = args[4]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_prepend(handle, *key, strlen(*key), *value, strlen(*value), expiration, flags)));
}

JSVAL _memcached_append(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	String::Utf8Value value(args[2]);
	time_t expiration = 0;
	if (args.Length() > 3) {
		expiration = args[3]->IntegerValue();
	}
	uint32_t flags = 0;
	if (args.Length() > 4) {
		flags = args[4]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_append(handle, *key, strlen(*key), *value, strlen(*value), expiration, flags)));
}

JSVAL _memcached_delete(JSARGS args) {
	HandleScope scope;
	M *handle = HANDLE(args[0]);
	String::Utf8Value key(args[1]);
	time_t expiration = 0;
	if (args.Length() > 2) {
		expiration = args[2]->IntegerValue();
	}
	return scope.Close(Integer::New(memcached_delete(handle, *key, strlen(*key), expiration)));
}


void init_memcached_object() {
	HandleScope scope;

	Handle<ObjectTemplate>memcached = ObjectTemplate::New();
	// constants
	memcached->Set(String::New("DEFAULT_PORT"),Integer::New(MEMCACHED_DEFAULT_PORT));
	memcached->Set(String::New("VERSION"), String::New(LIBMEMCACHED_VERSION_STRING));
	
	// methods
	memcached->Set(String::New("connect"), FunctionTemplate::New(_memcached_connect));
	memcached->Set(String::New("close"), FunctionTemplate::New(_memcached_close));
	memcached->Set(String::New("error"), FunctionTemplate::New(_memcached_error));
	
	memcached->Set(String::New("get"), FunctionTemplate::New(_memcached_get));
	memcached->Set(String::New("mget"), FunctionTemplate::New(_memcached_mget));
	memcached->Set(String::New("set"), FunctionTemplate::New(_memcached_set));
	memcached->Set(String::New("add"), FunctionTemplate::New(_memcached_add));
	memcached->Set(String::New("replace"), FunctionTemplate::New(_memcached_replace));
	memcached->Set(String::New("prepend"), FunctionTemplate::New(_memcached_prepend));
	memcached->Set(String::New("append"), FunctionTemplate::New(_memcached_append));
	memcached->Set(String::New("delete"), FunctionTemplate::New(_memcached_delete));

	builtinObject->Set(String::New("memcached"), memcached);
}
