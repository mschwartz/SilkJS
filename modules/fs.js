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
                   files.push(file);
               } 
            });
        }
        else {
            var entries = fs.readDir(dir);
            if (entries) {
                entries.each(function(file) {
                    files.push(file);
                });
            }
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
    },
    
    removeDirectory: function(dir) {
        function recurse(path) {
            console.log(path);
            var files = fs.readDir(path);
            if (files) {
                files.each(function(file) {
                    if (fs.isDir(path+'/'+file)) {
                        recurse(dir + '/' + path);
                        fs.rmdir(path);
                    }
                    else {
                        fs.unlink(path + '/' + file);
                        return;
                    }
                });
            }
        }
        if (!fs.isDir(dir)) {
            return;
        }
        recurse(dir);
        fs.rmdir(dir);
    }
});

exports.extend(fs);
