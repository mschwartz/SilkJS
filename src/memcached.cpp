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
 * DEFAULT_PORT: the default port for connecting to the memcached servers.
 * VERSION: the libmemcached version string.
 * 
 * ### Description
 * 
 * Some methods return an object or an object/hash of objects.  Typically these methods get values from the memcached store.
 * 
 * These objects are of the form:
 * 
 * + value: the string value returned from memcached
 * + flags: the 32-bit integer value stored with the value returned from memcached
 * + rc: memcached result code
 */
#include "SilkJS.h"
#include <libmemcached/memcached.h>

#define M memcached_st
#ifdef memcached_return_t
#define R memcached_return_t
#else
#define R memcached_return
#endif
#define S memcached_server_st

static inline M* HANDLE (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    return (M *) JSEXTERN(v);
}

/**
 * @function memcached.connect
 * 
 * ### Synopsis
 * 
 * var handle = memcached.connect(servers);
 * 
 * Create a connection to the specified memcached servers.
 * 
 * The servers argument is a comma separated list of memcached servers.  The server names are of the form <code>hostname[:port]</code>.
 * 
 * @param {string} servers - comma separated list of servers
 * @return {object} handle - opaque handle used for other memcached methods, or false if an error occurred.
 */
JSVAL _memcached_connect (JSARGS args) {
    HandleScope scope;
    String::Utf8Value options(args[0]);
    M *handle = memcached_create(NULL);
    S *servers = memcached_servers_parse(*options);
    if (memcached_server_push(handle, servers) != MEMCACHED_SUCCESS) {
        memcached_server_list_free(servers);
        memcached_free(handle);
        return scope.Close(False());
    }
    memcached_server_list_free(servers);
    return scope.Close(External::New(handle));
}

/**
 * @function memcached.close
 * 
 * ### Synopsis
 * 
 * memcached.close(handle);
 * 
 * Close an opened memcached handle and free all resources used for it.
 * 
 * @param {object} handle - handle to memcached connection.
 */
JSVAL _memcached_close (JSARGS args) {
    HandleScope scope;
    M* handle = HANDLE(args[0]);
    memcached_free(handle);
    return Undefined();
}

/**
 * @function memcached.error
 * 
 * ### Synopsis
 * var msg = memcached.error(handle, rc);
 * 
 * Get error message for last memcached error.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {int} rc - return code from the memcached that failed.
 * @return {string} msg - text of the error message.
 */
JSVAL _memcached_error (JSARGS args) {
    HandleScope scope;
    M* handle = HANDLE(args[0]);
    R rc = (R) args[1]->IntegerValue();
    return scope.Close(String::New(memcached_strerror(handle, rc)));
}

/**
 * @function memcached.get
 * 
 * ### Synopsis
 * 
 * var o = memcached.get(handle, key);
 * 
 * Get a value from memcached by key.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to get from memcached
 * @return {object} o - see description at top of the page, or false if an error occurred.
 */
JSVAL _memcached_get (JSARGS args) {
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

/**
 * @function memcached.mget
 * 
 * ### Synopsis:
 * 
 * var o = memcache.get(handle, array_of_keys);
 * 
 * Get multiple values, identified by an array of keys, from memcached.
 * 
 * The returned object is a hash of returned values, indexed by the key.  
 * 
 * For each of these keys, the value is an object in the form described at the top of this page.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {array} keys - array of keys of data to get from memcached
 * @return {object} o - has of objects of the form described at top of the page, or false if an error occurred.
 */
JSVAL _memcached_mget (JSARGS args) {
    HandleScope scope;
    M* handle = HANDLE(args[0]);
    Handle<Array> aKeys = Handle<Array>::Cast(args[1]);
    int numKeys = aKeys->Length();
    char *keys[numKeys];
    size_t key_lengths[numKeys];
    for (int i = 0; i < numKeys; i++) {
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

/**
 * @function memcached.set
 * 
 * ### Synopsis
 * 
 * var rc = memcached.set(handle, key, value);
 * var rc = memcached.set(handle, key, value, expiration);
 * var rc = memcached.set(handle, key, value, expiration, flags);
 * 
 * Store information in memcached indexed by key.  
 * 
 * If an object identified by key already exists on the server, it wil be replaced.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @param {string} value - value of data to set in memcached.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @param {int} flags - user defined integer value stored along with the value.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_set (JSARGS args) {
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

/**
 * @function memcached.add
 * 
 * ### Synopsis
 * 
 * var rc = memcached.add(handle, key, value);
 * var rc = memcached.add(handle, key, value, expiration);
 * var rc = memcached.add(handle, key, value, expiration, flags);
 * 
 * Store information in memcached indexed by key.  
 * 
 * If an object identified by key already exists on the server, an error occurs.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @param {string} value - value of data to set in memcached.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @param {int} flags - user defined integer value stored along with the value.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_add (JSARGS args) {
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

/**
 * @function memcached.replace
 * 
 * ### Synopsis
 * 
 * var rc = memcached.replace(handle, key, value);
 * var rc = memcached.replace(handle, key, value, expiration);
 * var rc = memcached.replace(handle, key, value, expiration, flags);
 * 
 * Store information in memcached indexed by key.  
 * 
 * If an object identified by key already exists on the server, it is replaced.  Otherwise an error occurs.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @param {string} value - value of data to set in memcached.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @param {int} flags - user defined integer value stored along with the value.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_replace (JSARGS args) {
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

/**
 * @function memcached.prepend
 * 
 * ### Synopsis
 * 
 * var rc = memcached.prepend(handle, key, value);
 * var rc = memcached.prepend(handle, key, value, expiration);
 * var rc = memcached.prepend(handle, key, value, expiration, flags);
 * 
 * Prepends the given value string to the value of an existing item.  
 * 
 * If an object identified by key does not exist, an error occurs.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @param {string} value - value of data to prepend in memcached.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @param {int} flags - user defined integer value stored along with the value.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_prepend (JSARGS args) {
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

/**
 * @function memcached.append
 * 
 * ### Synopsis
 * 
 * var rc = memcached.append(handle, key, value);
 * var rc = memcached.append(handle, key, value, expiration);
 * var rc = memcached.append(handle, key, value, expiration, flags);
 * 
 * Appends the given value string to the value of an existing item.  
 * 
 * If an object identified by key does not exist, an error occurs.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @param {string} value - value of data to append in memcached.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @param {int} flags - user defined integer value stored along with the value.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_append (JSARGS args) {
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

/**
 * @function memcached.remove
 * 
 * ### Synopsis
 * 
 * var rc = memcached.remove(handle, key);
 * 
 * Remove an item from memcached by key.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {string} key - key of data to set in memcached.
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_remove (JSARGS args) {
    HandleScope scope;
    M *handle = HANDLE(args[0]);
    String::Utf8Value key(args[1]);
    time_t expiration = 0;
    if (args.Length() > 2) {
        expiration = args[2]->IntegerValue();
    }
    return scope.Close(Integer::New(memcached_delete(handle, *key, strlen(*key), expiration)));
}

/**
 * @function memcached.flush
 * 
 * ### Synopsis
 * 
 * var rc = memcached.flush(handle);
 * var rc = memcached.flush(handle, expiration);
 * 
 * Wipe clean the contents of memcached servers.  It will either do this immediately or expire the content based on the expiration time passed to the method (a value of zero causes an immediate flush). The operation is not atomic to multiple servers, just atomic to a single server. That is, it will flush the servers in the order that they were added.
 * 
 * @param {object} handle - handle to memcached connection.
 * @param {int} expiration - length of time value is valid (after this it will be removed from memcached automatically).
 * @return {int} rc - result code; 0 if no error, otherwise the error code.
 */
JSVAL _memcached_flush (JSARGS args) {
    HandleScope scope;
    M *handle = HANDLE(args[0]);
    time_t expiration = 0;
    if (args.Length() > 1) {
        expiration = args[1]->IntegerValue();
    }
    return scope.Close(Integer::New(memcached_flush(handle, expiration)));
}

struct CTX {
    int ndx;
    Handle<Array>keys;
};
static memcached_return_t dump_fn(const memcached_st *ptr, const char *key, size_t key_length, void *context) {
    CTX *ctx = (CTX *)context;
    ctx->keys->Set(ctx->ndx++, String::New(key, key_length));
    return MEMCACHED_SUCCESS;
}

JSVAL _memcached_keys (JSARGS args) {
    M *handle = HANDLE(args[0]);
    CTX ctx;
    ctx.ndx = 0;
    ctx.keys = Array::New();
    memcached_dump_fn callbacks[1];
    callbacks[0] = &dump_fn;
    memcached_dump(handle, callbacks, &ctx, 1);
    return ctx.keys;
}

void init_memcached_object () {
    HandleScope scope;

    Handle<ObjectTemplate>memcached = ObjectTemplate::New();
    // constants
    memcached->Set(String::New("DEFAULT_PORT"), Integer::New(MEMCACHED_DEFAULT_PORT));
    memcached->Set(String::New("VERSION"), String::New(LIBMEMCACHED_VERSION_STRING));
    memcached->Set(String::New("SUCCESS"), Integer::New(MEMCACHED_SUCCESS));

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
    memcached->Set(String::New("remove"), FunctionTemplate::New(_memcached_remove));
    memcached->Set(String::New("flush"), FunctionTemplate::New(_memcached_flush));
    memcached->Set(String::New("keys"), FunctionTemplate::New(_memcached_keys));

    builtinObject->Set(String::New("memcached"), memcached);
}
