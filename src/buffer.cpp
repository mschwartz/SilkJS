/**
 * @module builtin/buffer
 * 
 * ### Synopsis
 * SilkJS builtin buffer object.
 * 
 * ### Description
 * The builtin/buffer object provides a growable byte buffer for strings and binary data.
 * 
 * Buffers should be treated as opaque handles in JavaScript code.  The builtin/buffer functions take this opaque handle as the first argument.  The exception, of course, is the buffer.create() method, which creates a new buffer.
 * 
 * ### Usage
 * var buffer = require('builtin/buffer');
 * 
 * ### See Also
 * builtin/net.writeBuffer()
 */
#include "SilkJS.h"

static inline void bufferWrite (Buffer *buf, const char *data, long len) {
#ifdef BUFFER_STRING
    buf->s += data;
#else
    if (buf->pos + len >= buf->size) {
        while (buf->pos + len >= buf->size) {
            buf->size *= 2;
        }
        buf->mem = (unsigned char *) realloc(buf->mem, buf->size);
    }
    memcpy(&buf->mem[buf->pos], data, len);
    buf->pos += len;
    buf->mem[buf->pos] = '\0';
#endif
}

/**
 * @function buffer.create
 * 
 * ### Synopsis
 * 
 * var buf = buffer.create();
 * 
 * Creates a new buffer.  Buffers created with buffer.create() should be released by calling buffer.destroy() when you are finished with the buffer.
 * 
 * @return {object} buf - opaque handel to newly created buffer.
 */
static JSVAL buffer_create (JSARGS args) {
    Buffer *buf = new Buffer;
#ifndef BUFFER_STRING
    buf->mem = new unsigned char[16384];
    buf->mem[0] = '\0';
    buf->size = 16384;
    buf->pos = 0;
#endif
    return Opaque::New(buf);
}

/**
 * @function buffer.reset
 * 
 * ### Synopsis
 * 
 * buffer.reset(buf);
 * 
 * Reset a buffer to "empty" state.
 * 
 * @param {object} buf - buffer to reset
 */
static JSVAL buffer_reset (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
#ifdef BUFFER_STRING
    buf->s.clear();
#else
    buf->pos = 0;
    buf->mem[0] = '\0';
#endif
    return Undefined();
}

/**
 * @function buffer.destroy
 * 
 * ### Synopsis
 * 
 * buffer.destroy(buf);
 * 
 * Free a previously created buffer.  Buffers allocate system resources that will not be freed until this function is called.
 * 
 * @param {object} buf - buffer to free.
 */
static JSVAL buffer_destroy (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
#ifndef BUFFER_STRING
    delete[] buf->mem;
#endif
    delete buf;
    return Undefined();
}

/**
 * @function buffer.write
 * 
 * ### Synopsis
 * buffer.write(buf, str);
 * 
 * Write a string into the specified buffer.
 * 
 * @param {object} buf - buffer to write string to.
 * @param {string{ str - string to add to buffer.
 */
static JSVAL buffer_write (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
    String::Utf8Value data(args[1]);
    bufferWrite(buf, *data, data.length());
    //#ifdef BUFFER_STRING
    //	buf->s += *data;
    //#else
    //	long len = args[2]->IntegerValue();
    //    if (buf->pos + len >= buf->size) {
    //		while (buf->pos + len >= buf->size) {
    //			buf->size *= 2;
    //		}
    //		buf->mem = (unsigned char *)realloc(buf->mem, buf->size);
    //    }
    //    memcpy(&buf->mem[buf->pos], *data, len);
    //    buf->pos += len;
    //    buf->mem[buf->pos] = '\0';
    //#endif
    return Undefined();
}

/**
 * @function buffer.write64
 * 
 * ### Synopsis
 * 
 * buffer.write64(buf, base64String)
 * 
 * Write a base64 encoded string to buffer, decoding the base64 to binary.  This function writes binary data to the buffer.
 * 
 * @param {object} buf - buffer to write to.
 * @param {string} base64Str - base64 encoded binary data to add to buffer as binary.
 */
static JSVAL buffer_write64 (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
    String::Utf8Value data(args[1]);
#ifdef BUFFER_STRING
    buf->s += Base64Decode(*data);
#else
    char decodeBuf[data.length()];
    long decodeLen = decode_base64((unsigned char *) decodeBuf, *data);
    bufferWrite(buf, decodeBuf, decodeLen);
#endif
    return Undefined();
}

/**
 * @function buffer.read
 * 
 * var contents = buffer.read(buf);
 * 
 * Return contents of buffer as a JavaScript string.
 * 
 * @param {object} buf - buffer to get contents of.
 * @return {string s - buffer contents as JavaScript string.
 */
static JSVAL buffer_read (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
#ifdef BUFFER_STRING
    return String::New(buf->s.c_str(), buf->s.size());
#else
    return String::New((char *) buf->mem, buf->pos);
#endif
}

/**
 * @function buffer.size
 * 
 * ### Synopsis
 * 
 * var size = buffer.size(buf);
 * 
 * Get count of bytes in a buffer.
 * 
 * @param {object} buf - buffer to get size of.
 * @return {int} size - size of buffer in bytes.
 */
static JSVAL buffer_size (JSARGS args) {
    Buffer *buf = (Buffer *)JSOPAQUE(args[0]);
#ifdef BUFFER_STRING
    return Integer::New(buf->s.size());
#else
    return Integer::New(buf->pos);
#endif
}

void init_buffer_object () {
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
