/**
 * @module MimeTypes
 * 
 * ### Synopsis
 * 
 * var mimeTypes = require('MimeTypes');
 * 
 * ### Description
 * 
 * MimeTypes is a hash/object.  The key in the object is file extension, the value is MIME type (e.g. HTTP content-type value).
 * 
 * The system mime.types file is loaded, if it can be found.  For Linux, the file is expected to exist in /etc/mime.types.  For OSX, it's expected in /Private/etc/apache2/mime.types.
 * 
 * ### Throws
 * This module throws an error at require() time if the mime.types file cannot be found.
 */

 // locate mime.types file

"use strict";

var fs = require('fs');
 
var filename = null;

[
    '/etc/mime.types',
    '/Private/etc/apache2/mime.types'
].each(function(path) {
    if (fs.exists(path)) {
        filename = path;
    }
});

if (!filename) {
    error('MimeTypes module: Cannot find mime.types file.');
}

var mimeTypes = {};
var lines = fs.readFile(filename).split(/\n/);
lines.each(function(line) {
    line = line.trim();
    if (line.length == 0 || line.startsWith('#')) {
        return;
    }
    var parts = line.split(/\s+/);
    if (parts.length == 1) {
        return;
    }
    var contentType = parts.shift();
    parts.each(function(extension) {
        mimeTypes[extension] = contentType;
    });
});

exports = mimeTypes;
