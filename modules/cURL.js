/**
 * @module cURL
 * 
 * This file implements the JavaScript version of cURL for doing HTTP to remote servers.
 * 
 * ### Synopsis:
 * 
 * var curl = require('cURL');
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
                c.setMethod(handle, config.method);
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
