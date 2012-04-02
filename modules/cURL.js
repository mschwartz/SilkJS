/**
 * @module cURL
 * 
 * This file implements the JavaScript version of cURL for doing HTTP to remote servers.
 * 
 * ### Synopsis:
 * 
 * var cURL = require('cURL');
 * 
 * ### Note
 * 
 * The name of the function exported by this module is simply cURL.  You do not need to call cURL.cURL()...
 * 
 */

var c = require('builtin/curl');

function isObject(v) {
    return !!v && Object.prototype.toString.call(v) === '[object Object]';
}
function isArray(v) {
    return toString.apply(v) === '[object Array]';
}
function isString(v) {
    return typeof v === 'string';
}

/**
 * @function cURL
 * 
 * ### Synopsis
 * 
 * var result = cURL(config);
 * 
 * Perform a cURL request.
 * 
 * The config parameter is an object of the form:
 * 
 * + url: (required) the URL to POST or GET from the remote server.
 * + method: (optional) either "GET" or "POST"
 * + followRedirects (optional) true (default) to follow redirect responses from the server
 * + cookies: (optional) Object, Array, or String representation of cookie header to send
 * + headers: (optional) Object, Array, or String representation of HTTP headers to add to the request.
 * + params: (valid for POST only): POST variables to send to server
 * 
 * The result returned is an object of the form:
 * 
 * + status: HTTP status from server (e.g. 200 for OK, 404 for not found, etc.)
 * + contentType: the MIME type or content-type of the response.
 * + responseText: the response from the server as a string.
 * 
 * @param {object} config - a config object as described above.
 * @returns {object} result - a result object as described above.
 * 
 * ### Note
 * This function throws an error if the arguments are invalid.
 */
function cURL(config) {
    if (!config.url) {
        error('url required');
    }
    var method = 'GET';
    
    var handle = c.init(config.url);
    if (config.method) {
        switch (config.method.toLowerCase()) {
            case 'post':
                method = 'POST';
            case 'get':
//                c.setMethod(handle, config.method);
                break;
            default:
                error('GET/POST are only allowed methods.');
        }
    }
    if (config.followRedirects) {
        c.followRedirects(handle, config.followRedirects ? 1 : 0);
    }
    if (config.cookies) {
        var cookies = [];
        if (isObject(config.cookies)) {
            config.cookies.each(function(value, key) {
                cookies.push(key + '=' + encodeURIComponent(value));
            });
            c.setCookie(handle, cookies.join('; '));
        }
        else if (isArray(config.cookies)) {
            c.setCookie(handle, config.cookies.join('; '));
        }
        else if (isString(config.cookies)) {
            c.setCookie(handle, config.cookies);
        }
        else {
            error('Invalid cookies config');
        }
    }
    if (config.params) {
        var paramString = '';
        if (isObject(config.params)) {
            var params = [];
            config.params.each(function(value, key) {
                params.push(key + '=' + encodeURIComponent(value));
            });
            paramString = params.join('&');
        }
        else if (isArray(config.params)) {
            paramString = config.params.join('&');
        }
        else if (isString(config.params)) {
            paramString = config.params;
        }
        else {
            error('Invalid params config');
        }
        switch (method) {
            case 'POST':
                c.setPostFields(handle, paramString);
                break;
            default:
                error('The params config is only valid for POST');
        }
    }
    
    if (config.headers) {
        if (isObject(config.headers)) {
            config.headers.each(function(value, key) {
                c.setHeader(handle, key + ': ' + value);
            });
        }
        else if (isArray(config.headers)) {
            config.headers.each(function(value) {
                c.setHeader(handle, value);
            });
        }
        else if (isString(config.headers)) {
            var headers = config.headers.split('\n');
            headers.each(function(header) {
                c.setHeader(handle, header);
            });
        }
        else {
            error('Invalid headers config');
        }
    }
    
    var errorCode = c.perform(handle);
    if (errorCode) {
        error(c.error(errorCode));
    }
    var ret =  {
        status: c.getResponseCode(handle),
        contentType: c.getContentType(handle),
        responseText: c.getResponseText(handle)
    };
    c.destroy(handle);
    return ret;
};

if (exports) {
    exports = cURL;
}
