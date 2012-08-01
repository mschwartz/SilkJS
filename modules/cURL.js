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
 * + params: POST variables to send to server
 * + form: FORM to send to server (see below)
 * + verbose: set to > 0 to have cURL library log debugging info to console
 * + timeout: set to number of seconds to have cURL timeout.  Defaults to infinite.
 *
 * NOTE: only one of form or params may be provided.
 *
 * If you wish to submit a multipart/form style form to the remote server, set the form config parameter, which is an array of objects of the following format:
 *
 * + name: (required) name of form field
 * + value: (required) value of form field
 * + fileUpload: (optional) if true, value is the name of a file to upload
 * + contentType: (optional) content-type header to set for this field
 *
 * The result returned is an object of the form:
 *
 * + status: HTTP status from server (e.g. 200 for OK, 404 for not found, etc.)
 * + contentType: the MIME type or content-type of the response.
 * + responseHeaders: an object with key/value pairs; the key is the header name, the value is the value.
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
        throw new Error('url required');
    }
    var method = 'GET';
    
    if (config.params && config.form) {
        throw new Error('Only one of params and form may be supplied');
    }
    
    var handle = c.init(config.url);
    if (config.method) {
        switch (config.method.toLowerCase()) {
            case 'post':
                method = 'POST';
                break;
            case 'get':
//                c.setMethod(handle, config.method);
                break;
            default:
                method = config.method.toUpperCase();
                c.setMethod(handle, method);
                break;
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
            throw new Error('Invalid cookies config');
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
            throw new Error('Invalid params config');
        }
        if (method !== 'GET') {
            c.setPostFields(handle, paramString);
        }
    }
    if (config.form) {
        config.form.each(function(field) {
            if (field.fileUpload) {
                if (field.contentType) {
                    c.addFormFile(handle, field.name, field.value, field.contentType);
                }
                else {
                    c.addFormFile(handle, field.name, field.value);
                }
            }
            else {
                if (field.contentType) {
                    c.addFormField(handle, field.name, field.value, field.contentType);
                }
                else {
                    c.addFormField(handle, field.name, field.value);
                }
            }
        });
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
            throw new Error('Invalid headers config');
        }
    }

    if (config.timeout !== undefined) {
        c.setTimeout(handle, config.timeout);
    }

    var errorCode = c.perform(handle, config.verbose || 0);
    if (errorCode) {
        throw new Error(c.error(errorCode));
    }
    var responseHeaders = {};
    var responseHeaderText = c.getResponseHeaders(handle).replace(/\r/igm, '');
    if (responseHeaderText.length) {
        responseHeaderText.split('\n').each(function(header) {
            if (header.length) {
                var parts = header.split(/: /);
                if (parts[0] && parts[1]) {
                    responseHeaders[parts[0].toLowerCase()] = parts[1];
                }
            }
        });
    }
    var ret =  {
        status: c.getResponseCode(handle),
        contentType: c.getContentType(handle),
        responseHeaders: responseHeaders,
        responseText: c.getResponseText(handle)
    };
    c.destroy(handle);
    return ret;
}

if (exports) {
    exports = cURL;
}
