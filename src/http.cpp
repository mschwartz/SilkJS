/**
 * @module builtin/http
 * 
 * ### Synopsis
 * SilkJS builtin http object.
 * 
 * ### Description
 * The builtin/http object provides a number of methods that are applicable to reading from the HTTP stream sent by a client.
 * 
 * One of the trickier parts of implementing an HTTP server in JavaScript is processing of MIME content that contains binary data.  The http Stream and methods process the mix of text and binary data into a mix of text and base64 encoded text, suitable for processing by JavaScript code.
 * 
 * A stream is buffered, using a native C++ buffer (see builtin/buffer) automatically.
 * 
 * ### Usage
 * var http = require('builtin/http');
 * 
 * ### See Also
 * The JavaScriptimplementation of the http server.
 */
#include "SilkJS.h"

/**
 * @function http.openStream
 * 
 * ### Synopsis
 * 
 * var stream = openStream(socket);
 * 
 * Create a stream parser from an existing socket.
 * 
 * ### Description
 * 
 * A stream parser is able to read both text and binary data from a socket.  The stream object returned should be treated as an opaque handle by JavaScript code.
 * 
 * @param {int} socket - file descriptor of socket to turn into a stream
 * @return {object} stream - opaque stream handle
 */
static JSVAL OpenStream (JSARGS args) {
    InputStream *s = new InputStream(args[0]->IntegerValue());
    return Opaque::New(s);
}

/**
 * @function http.closeStream
 * 
 * ### Synopsis
 * 
 * http.closeStream(stream);
 * 
 * Close an opened stream, freeing any resources it is using.  Does not close the socket.
 * 
 * @param {object} stream - opaque handle to the stream to close.
 */
static JSVAL CloseStream (JSARGS args) {
    InputStream *s = (InputStream *)JSOPAQUE(args[0]);
    delete s;
    return Undefined();
}

/**
 * @function http.readByte
 * 
 * ### Synopsis
 * 
 * var byte = http.readByte(stream);
 * 
 * Return the next byte from the input stream.
 * 
 * @param {object} stream - opaque handle to stream to read from
 * @return {int} byte - next byte from stream, or -1 on error or timeout.
 */
static JSVAL ReadByte (JSARGS args) {
    InputStream *s = (InputStream *)JSOPAQUE(args[0]);
    return Integer::New(s->Read());
}

/**
 * @function http.readHeaders
 * 
 * ### Synopsis
 * 
 * var headers = http.readHeaders(stream);
 * 
 * Read HTTP headers from the stream and return them as a single string.
 * 
 * HTTP headers are a consecutive number of lines (terminated by '\n').  The headers are terminated by two consecutive newlines.
 * 
 * @param {object} stream - opaque handle of stream to read headers from.
 * @return {string} headers - raw HTTP headers, as a string.  If no headers could be read, null is returned.
 */
static JSVAL ReadHeaders (JSARGS args) {
    InputStream *s = (InputStream *)JSOPAQUE(args[0]);

    string out;
    int newlineCount = 0;
    while (newlineCount < 2) {
        char c = s->Read();
        switch (c) {
            case 0:
            case -1: // connection closed
                //				if (out.size()) {
                //					break;
                //				}
                return Null();
            case '\r':
                continue;
            case '\n':
                newlineCount++;
                out += c;
                break;
            default:
                out += c;
                newlineCount = 0;
                break;
        }
    }
    return String::New(out.c_str(), out.size());
}

/**
 * @function http.readPost
 * 
 * ### Synopsis
 * 
 * var postString = http.readPost(stream, contentLength);
 * 
 * Read POST variables from the stream, returning a raw string.  Post variables are of the form key=val&key=val...
 * 
 * The caller is expected to parse the Content-Length header to determine the number of bytes that comprise the post variables string.
 * 
 * @param {object} stream - the stream to read POST headers from.
 * @param {int} contentLength - the value of Content-Length header; the number of bytes to read.
 * @return {string} postString - raw POST variables string.
 */
static JSVAL ReadPost (JSARGS args) {
    InputStream *s = (InputStream *)JSOPAQUE(args[0]);
    long size = args[1]->IntegerValue();

    char buf[size];
    if (s->Read((unsigned char *) buf, size) < 0) {
        return False();
    }
    Handle<String> out = String::New(buf, size);
    return out;
}

/**
 * @function http.readMime
 * 
 * ### Synopsis
 * 
 * var mimeString = http.readMime(stream, size, boundary);
 * 
 * Process multi-part/mime stream data from the specified stream.
 * 
 * ### Description
 * 
 * The caller will parse the Content-type header and call this function when the value is multi-part/mime.  The boundary string would also be parsed from this header.  The size value is parsed from the Content-length header by the caller.
 * 
 * This function reads the specified size number of bytes from the stream and transforms the bytes into a JavaScript string.  
 * 
 * The transformation process converts binary MIME parts into base64 encoded MIME parts, adding content-length and content-encoding headers to the returned string.
 * 
 * @param {object} stream - opaque handle to stream to read multi-part/mime data from.
 * @param {int} size - the content length of the multi-part/mime stream.
 * @param {string} boundary - the MIME part boundary string parsed from the Content-type header.
 * @return {string} mimeString - transformed multi-part/mime stream as a JavaScript string.
 */
static JSVAL ReadMime (JSARGS args) {
    InputStream *s = (InputStream *)JSOPAQUE(args[0]);
    long size = args[1]->IntegerValue();
    String::Utf8Value jsboundary(args[2]);
    char *boundary = *jsboundary;

    char *in = new char[size];
    if (s->Read((unsigned char *) in, size) < 0) {
        delete [] in;
        return Null();
    }

    char b[strlen(boundary) + 3];
    sprintf(b, "--%s", boundary);
    int bLength = strlen(b);
    char end[strlen(boundary) + 3 + 2];
    sprintf(end, "--%s--", boundary);
    //	int endLength = strlen(end);

    string out;

    long ndx = 0;
    while (ndx < size) {
        while (strncmp(&in[ndx], b, bLength)) {
            out += in[ndx++];
        }
        out.append(&in[ndx], bLength);
        ndx += bLength;

        if (!strncmp(&in[ndx], "--", 2)) {
            out += "--\r\n";
            ndx += 4;
            break;
        }
        else {
            out += "\r\n";
            ndx += 2;
        }
        // mime part header
        long lookAhead = ndx;
        while (in[lookAhead] != '\r' && (in[lookAhead] != '\n')) {
            out += in[lookAhead];
            lookAhead++;
        }
        char save = in[lookAhead];
        in[lookAhead] = '\0';
        if (strcasestr(&in[ndx], "filename=")) {
            in[lookAhead] = save;
            ndx = lookAhead;
            while (strncmp(&in[ndx], "\r\n\r\n", 4)) {
                out += in[ndx++];
            }
            out += "\r\n";
            ndx += 4;
            // in[ndx] is start of binary data
            lookAhead = ndx;
            while (in[lookAhead] != '-' || strncmp(&in[lookAhead], b, bLength)) {
                lookAhead++;
            }
            // in[lookAhead] is start of boundary
            string base64 = Base64Encode((unsigned char *) &in[ndx], lookAhead - ndx - 2);
            char buf[512];
            sprintf(buf, "Content-Length: %ld\r\n", lookAhead - ndx - 2);
            out.append(buf, strlen(buf));
            strcpy(buf, "Content-Encoding: base64\r\n");
            out.append(buf, strlen(buf));
            out += "\r\n";
            out += base64;
            out += "\r\n";
            ndx = lookAhead;
        }
        else {
            in[lookAhead] = save;
            ndx = lookAhead;
        }
    }
    delete [] in;
    return String::New(out.c_str());
}

void init_http_object () {
    Handle<ObjectTemplate>http = ObjectTemplate::New();
    http->Set(String::New("openStream"), FunctionTemplate::New(OpenStream));
    http->Set(String::New("closeStream"), FunctionTemplate::New(CloseStream));
    http->Set(String::New("readByte"), FunctionTemplate::New(ReadByte));
    http->Set(String::New("readHeaders"), FunctionTemplate::New(ReadHeaders));
    http->Set(String::New("readPost"), FunctionTemplate::New(ReadPost));
    http->Set(String::New("readMime"), FunctionTemplate::New(ReadMime));

    builtinObject->Set(String::New("http"), http);
}
