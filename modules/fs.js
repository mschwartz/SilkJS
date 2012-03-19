/**
 * @module fs
 * 
 * ### Synopsis
 * 
 * var fs = require('fs');
 * 
 * ### Description
 * 
 * This module extends the builtin fs module.
 */

"use strict";

var fs = require('builtin/fs');

function listRecursive(dir, pattern) {
    var files = [];
    var recurse = pattern ? function(dir) {
        fs.readDir(dir).each(function(file) {
            if (fs.isDir(dir + '/' + file)) {
                recurse(dir + '/' + file);
            }
            else if (pattern.test(file)) {
                files.push(dir + '/' + file);
            }
        });
    } : function(dir) {
        fs.readDir(dir).each(function(file) {
            if (fs.isDir(dir + '/' + file)) {
                recurse(dir + '/' + file);
            }
            else {
                files.push(dir + '/' + file);
            }
        });
    };
    recurse(dir);
    return files;
}

exports.extend(fs);
exports.extend({
    listRecursive: listRecursive
});
