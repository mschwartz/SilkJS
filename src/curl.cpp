/**
 * @module builtin/curl
 * 
 * ### Synopsis
 * SilkJS builtin curl object.
 * 
 * ### Description
 * The builtin/curl object provides an interface to the curl library.
 * 
 * ### Usage
 * var curl = require('builtin/curl');
 * 
 * ### See Also
 * The JavaScriptimplementation of a more robust console object:
 * modules/console.js
 */
#include "SilkJS.h"
#include <curl/curl.h>

struct CHANDLE {
    CURL *curl;
    char *memory;
    size_t size;
    char *headers;
    size_t hsize;
    curl_slist *slist;
    curl_httppost *post;
    curl_httppost *last;
};

static inline CHANDLE *HANDLE (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    CHANDLE *w = (CHANDLE *) JSOPAQUE(v);
    return w;
}

static size_t WriteMemoryCallback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct CHANDLE *w = (CHANDLE *) userp;
    w->memory = (char *) realloc(w->memory, w->size + realsize + 1);
    if (w->memory == NULL) {
        return -1;
    }
    memcpy(&w->memory[w->size], contents, realsize);
    w->size += realsize;
    w->memory[w->size] = '\0';
    return realsize;
}

static size_t WriteHeaderCallback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *s = (char *)contents;
    if (s[0] == '\n') {
        return realsize;
    }
    struct CHANDLE *w = (CHANDLE *) userp;
    w->headers = (char *) realloc(w->headers, w->hsize + realsize + 1);
    if (w->headers == NULL) {
        return -1;
    }
    memcpy(&w->headers[w->hsize], contents, realsize);
    w->hsize += realsize;
    w->headers[w->hsize] = '\0';
    return realsize;
}

/**
 * @function curl.error
 * 
 * ### Synopsis
 * 
 * var msg = curl.error(code);
 * 
 * Returns a string describing the CURLcode error code passed in the argument code.
 * 
 * @param {int} code - CURL error code.
 * @return {string} msg - string describing the error.
 */
static JSVAL error (JSARGS args) {
    CURLcode eNumber = (CURLcode) args[0]->IntegerValue();
    return String::New(curl_easy_strerror(eNumber));
}

/**
 * @function curl.init
 * 
 * ### Synopsis
 * 
 * var handle = curl.init(url);
 * 
 * Initialize a curl connection to the specified URL.  By default, redirects will be followed.  The returned handle should be considered opaque by JavaScript code.  The actual request is not performed until the curl.perform() method is called on the handle.
 * 
 * You must call curl.destroy() on the returned handle to free the resources allocated for the CURL request.
 * 
 * @param {string} url - the URL for the connection.
 * @return {object} handle - opaque handle
 */
static JSVAL init (JSARGS args) {
    String::Utf8Value url(args[0]->ToString());
    CURL *curl = curl_easy_init();
    if (!curl) {
        return String::New("Could not initialize CURL library");
    }
    struct CHANDLE *w = new CHANDLE;
    w->curl = curl;
    w->memory = (char *) malloc(1);
    w->size = 0;
    w->headers = (char *) malloc(1);
    w->hsize = 0;
    w->slist = NULL;
    w->post = NULL;
    w->last = NULL;
    curl_easy_setopt(curl, CURLOPT_URL, *url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) w);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WriteHeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *) w);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "SilkJS/1.0");
    return Opaque::New(w);
}

/**
 * @function curl.setMethod
 * 
 * var status = curl.setMethod(handle, method);
 * 
 * Sets the method for the CURL request to either GET or POST.
 * 
 * @param {object} handle - CURL handle.
 * @param {string} method - may be either "GET" or "POST".
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL setMethod (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value method(args[1]->ToString());
    if (!strcasecmp(*method, "post")) {
        return Integer::New(curl_easy_setopt(h->curl, CURLOPT_POST, 1));
    }
    else if (!strcasecmp(*method, "get")) {
        return Integer::New(curl_easy_setopt(h->curl, CURLOPT_HTTPGET, 1));
    }
    else {
        return Integer::New(curl_easy_setopt(h->curl, CURLOPT_CUSTOMREQUEST, *method));
    }
}

/**
 * @function curl.followRedirects
 * 
 * ### Synopsis
 * 
 * var status = curl.followRedirects(handle, flag);
 * 
 * Sets the "follow redirect" protocol for the specified handle to the value of flag.
 * 
 * @param {object} handle - CURL handle.
 * @param {int} flag - 1 to follow redirects, 0 to not follow redirects.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL followRedirects (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    long flag = (long) args[0]->IntegerValue();
    return Integer::New(curl_easy_setopt(h->curl, CURLOPT_FOLLOWLOCATION, flag));
}

/**
 * @function curl.setCookie
 * 
 * ### Synopsis
 * 
 * var status = curl.setCookie(handle, cookie_string);
 * 
 * Sets the cookie (header) for the CURL request.
 * 
 * The format of the string should be NAME=CONTENTS, where NAME is the cookie name and CONTENTS is what the cookie should contain.
 * 
 * If you need to set multiple cookies, you need to set them all using a single option and thus you need to concatenate them all in one single string. Set multiple cookies in one string like this: "name1=content1; name2=content2;" etc.
 *
 * This option sets the cookie header explicitly in the outgoing request(s). If multiple requests are done due to authentication, followed redirections or similar, they will all get this cookie passed on.
 * 
 * @param {object} handle - CURL handle.
 * @param {string} cookie_string - see comments above for the format.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL setCookie (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value cookie_string(args[1]->ToString());
    return Integer::New(curl_easy_setopt(h->curl, CURLOPT_COOKIE, *cookie_string));
}

/**
 * @function curl.setHeader
 * 
 * ### Synopsis
 * 
 * var status = curl.setHeader(handle, header_string);
 * 
 * Sets the specified header for the CURL request.
 * 
 * The format of the string should be NAME=CONTENTS, where NAME is the header name and CONTENTS is the value of the header.
 * 
 * This option sets the header explicitly in the outgoing request(s). 
 * 
 * @param {object} handle - CURL handle.
 * @param {string} header_string - see comments above for the format.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL setHeader (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value header_string(args[1]->ToString());
    h->slist = curl_slist_append(h->slist, *header_string);
    return Undefined();
}

/**
 * @function curl.addFormField
 * 
 * ### Synopsis
 * 
 * var status = curl.addFormField(handle, name, value);
 * var status = curl.addFormField(handle, name, value, contentType);
 * 
 * Add a section to a multipart/formdata HTTP POST.
 * 
 * ### Description
 * 
 * curl.addFormField() is used to append sections when building a multipart/formdata HTTP POST.  Append one section at a time until you've added all the sections you want included.
 * 
 * If you call this function, or addPostFile(), at least once, a POST will automatically occur when you call curl.perform().
 * 
 * Each part of a multipart/formdata post consists of at least a NAME and a CONTENTS part.  The second form of the call to this function allows you to set a content-type header for the part.
 * 
 * If you want to perform a file upload, you may add that part by calling curl.addPostFile().
 * 
 * @param {object} handle - CURL handle
 * @param {string} name - field name
 * @param {string} value - field value
 * @param {string} contentType - optional content-type for the field value.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL addFormField(JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value name(args[1]->ToString());
    String::Utf8Value value(args[2]->ToString());
    char *contentType = NULL;
    if (args.Length() > 3) {
        String::Utf8Value type(args[3]->ToString());
        contentType = *type;
    }
    if (contentType) {
        return Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_COPYCONTENTS, *value, CURLFORM_CONTENTTYPE, contentType, CURLFORM_END));
    }
    else {
        return Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_COPYCONTENTS, *value, CURLFORM_END));
    }
}

/**
 * @function curl.addFormFile
 * 
 * ### Synopsis
 * 
 * var status = curl.addFormFile(name, filename);
 * var status = curl.addFormFile(name, filename, contentType);
 * 
 * Add a file upload section to a multipart/formdata HTTP POST.
 * 
 * ### Description
 * 
 * curl.addFormFile() is used to append file upload sections when building a multipart/formdata HTTP POST.
 * 
 * If you call this function, or addPostField(), at least once, a POST will automatically occur when you call curl.perform().
 * 
 * The second form of the call to this function allows you to set a content-type header for the part.
 * 
 * @param {object} handle - CURL handle
 * @param {string} name - field name
 * @param {string} filename - path to existing file to upload
 * @param {string} contentType - optional content-type for the field value.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL addFormFile(JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value name(args[1]->ToString());
    String::Utf8Value filename(args[2]->ToString());
    char *contentType = NULL;
    if (args.Length() > 3) {
        String::Utf8Value type(args[3]->ToString());
        contentType = *type;
    }
    if (contentType) {
        return Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_FILE, *filename, CURLFORM_CONTENTTYPE, contentType, CURLFORM_END));
    }
    else {
        return Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_FILE, *filename, CURLFORM_END));
    }
}

/**
 * @function curl.setPostFields
 * 
 * ### Synopsis
 * 
 * var status = curl.setPostFields(handle, post_fields);
 * 
 * Set the post fields for the CURL request.
 * 
 * The post_fields string should be the full data to post in a HTTP POST operation. You must make sure that the data is formatted the way you want the server to receive it. libcurl will not convert or encode it for you. Most web servers will assume this data to be url-encoded.
 * 
 * The pointed data are NOT copied by the library: as a consequence, they must be preserved by the calling application until the transfer finishes.
 * This POST is a normal application/x-www-form-urlencoded kind (and libcurl will set that Content-Type by default when this option is used), which is the most commonly used one by HTML forms.
 * 
 * @param {object} handle - CURL handle.
 * @param {string} cookie_string - see comments above for the format.
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL setPostFields (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    String::Utf8Value post_fields(args[1]->ToString());
    //    printf("%d %s\n", strlen(*post_fields), *post_fields);
    curl_easy_setopt(h->curl, CURLOPT_POSTFIELDSIZE, strlen(*post_fields));
    return Integer::New(curl_easy_setopt(h->curl, CURLOPT_COPYPOSTFIELDS, *post_fields));
}

/**
 * @function curl.setTimeout
 *
 * ### Synopsis
 *
 * curl.setTimeout(handle, seconds);
 * 
 * Set timeout for CURL operations.  Normally they do not timeout, which can cause a hang to occur.
 * @param {object} handle - CURL handle
 * @param {int} seconds - timeout in seconds, or 0 for infinite.
 */
static JSVAL setTimeout (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    long timeout = args[1]->IntegerValue();
    curl_easy_setopt(h->curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(h->curl, CURLOPT_TIMEOUT, timeout);
    return Undefined();
}

/**
 * @function curl.perform
 * 
 * ###Synopsis
 * 
 * var status = curl.perform(handle);
 * var status = curl.perform(handle, verbose);
 * 
 * Perform the CURL request.
 * 
 * This function is called after the curl.init() and all the method calls to set options are made, and will perform the transfer as described in the options. It must be called with the same handle as input as the curl.init() call returned.
 * 
 * @param {object} handle - CURL handle
 * @param {int} verbose - set to > 0 to have cURL library print debugging info to console
 * @return {int} status - 0 for success, otherwise an error code.
 */
static JSVAL perform (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    if (h->slist) {
        curl_easy_setopt(h->curl, CURLOPT_HTTPHEADER, h->slist);
    }
    if (h->post) {
        curl_easy_setopt(h->curl, CURLOPT_HTTPPOST, h->post);
    }
    if (args.Length() > 1) {
        curl_easy_setopt(h->curl, CURLOPT_VERBOSE, args[1]->IntegerValue());
    }
    return Integer::New(curl_easy_perform(h->curl));
}

/**
 * @function curl.getResponseCode
 * 
 * ### Synopsis
 * 
 * var code = curl.getResponseCode(handle);
 *  
 * Get the response code (e.g. 200 for OK, 404 for not found, etc.) for a completed CURL request.
 * 
 * @param {object} handle - CURL handle
 * @return {int} status - 200 for OK, etc.
 */
static JSVAL getResponseCode (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    long status;
    curl_easy_getinfo(h->curl, CURLINFO_RESPONSE_CODE, &status);
    return Integer::New(status);
}

/**
 * @function curl.getContentType
 * 
 * ### Synopsis
 * 
 * var contentType = curl.getContentType(handle);
 * 
 * Gets the content-type: header value for a completed response.  This is the MIME type of the response.
 * 
 * @param {object} handle - CURL handle
 * @return {string} contentType - MIME string / content-type
 */
static JSVAL getContentType (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    char *contentType;
    curl_easy_getinfo(h->curl, CURLINFO_CONTENT_TYPE, &contentType);
    if (!contentType) {
        contentType = "";
    }
    return String::New(contentType);
}

/**
 * @funciton curl.getResponseHeaders
 *
 * ### Synopsis
 *
 * var responseHeaders = curl.getResponseHeaders(handle);
 *
 * Gets the response headers of the completed CURL response.
 *
 * The response headers are returned in the form of a string; each header line is terminated with a newline.
 *
 * @param {object} handle - CURL handle
 * @return {string} responseHeaders - the response headers of the completed CURL request.
 */
static JSVAL getResponseHeaders (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    if (!h->hsize) {
        return String::New("");
    }
    return String::New(h->headers, h->hsize);
}

/**
 * @function curl.getResponseText
 * 
 * ### Synopsis
 * 
 * var responseText = curl.getResponseText(handle);
 * 
 * Gets the responseText of the completed CURL response.  This does not include any headers.
 * 
 * @param {object} handle - CURL handle
 * @return {string} responseText - the response text of the completed CURL request.
 */
static JSVAL getResponseText (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    if (!h->size) {
        return String::New("{}");
    }
    return String::New(h->memory, h->size);
}

/**
 * @function curl.destroy(handle);
 * 
 * ### Synopsis
 * 
 * Frees memory allocated by curl.init(), including the handle.  The handle will no longer be valid.
 * 
 * @param {object} handle - CURL handle
 */
static JSVAL destroy (JSARGS args) {
    CHANDLE *h = HANDLE(args[0]);
    if (h->slist) {
        curl_slist_free_all(h->slist);
        h->slist = NULL;
    }
    if (h->post) {
        curl_formfree(h->post);
    }
    curl_easy_cleanup(h->curl);
    free(h->memory);
    free(h->headers);
    free(h);
    return Undefined();
}

void init_curl_object () {
    JSOBJT curlObject = ObjectTemplate::New();
    curlObject->Set(String::New("error"), FunctionTemplate::New(error));
    curlObject->Set(String::New("init"), FunctionTemplate::New(init));
    curlObject->Set(String::New("setMethod"), FunctionTemplate::New(setMethod));
    curlObject->Set(String::New("followRedirects"), FunctionTemplate::New(followRedirects));
    curlObject->Set(String::New("setCookie"), FunctionTemplate::New(setCookie));
    curlObject->Set(String::New("setHeader"), FunctionTemplate::New(setHeader));
    curlObject->Set(String::New("addFormField"), FunctionTemplate::New(addFormField));
    curlObject->Set(String::New("addFormFile"), FunctionTemplate::New(addFormFile));
    curlObject->Set(String::New("setPostFields"), FunctionTemplate::New(setPostFields));
    curlObject->Set(String::New("setTimeout"), FunctionTemplate::New(setTimeout));
    curlObject->Set(String::New("perform"), FunctionTemplate::New(perform));
    curlObject->Set(String::New("getResponseCode"), FunctionTemplate::New(getResponseCode));
    curlObject->Set(String::New("getContentType"), FunctionTemplate::New(getContentType));
    curlObject->Set(String::New("getResponseText"), FunctionTemplate::New(getResponseText));
    curlObject->Set(String::New("getResponseHeaders"), FunctionTemplate::New(getResponseHeaders));
    curlObject->Set(String::New("destroy"), FunctionTemplate::New(destroy));

    builtinObject->Set(String::New("curl"), curlObject);
}

