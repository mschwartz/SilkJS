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

exports.extend({
    list: function(dir, pattern) {
        var files = [];
        if (pattern) {
            fs.readDir(dir).each(function(file) {
               if (pattern.test(file)) {
                   files.push(dir + '/' + file);
               } 
            });
        }
        else {
            fs.readDir.each(function(file) {
                files.push(dir + '/' + file);
            });
        }
        return files;
    },
    
    listRecursive: function(dir, pattern) {
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
});

exports.extend(fs);
