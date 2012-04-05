/**
 * @module fs
 * 
 * ### Synopsis
 * 
 * var fs = require('fs');
 * 
 * ### Description
 * 
 * This module extends the builtin fs module.  All of the builtin/fs methods are available through this module, except where the functions defined here replace those in the builtin version.
 * 
 * ### See Also
 * builtin/fs module
 */

"use strict";

var fs = require('builtin/fs'),
    process = require('builtin/process');

var newfs = {
	/**
	 * @function fs.list
	 * 
	 * ### Synopsis
	 * 
	 * var files = fs.list(dir, pattern);
	 * var files = fs.list(dir);
	 * 
	 * List a directory, returning an array of file or directory names.
	 * 
	 * If the optional pattern paramter is supplied, only files that match this regex will be included in the result array.
	 * 
	 * @param {string} dir - path to directory to get list of files for.
	 * @param {regex} pattern - pattern to filter list of returned files.
	 * @returns {array} files - array of file names.
	 */
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
    
	/**
	 * @function fs.listRecursive
	 * 
	 * ### Synopsis
	 * 
	 * var files = fs.list(dir, pattern);
	 * var files = fs.list(dir);
	 * 
	 * List a directory, recursively, returning an array of file names.
	 * 
	 * If the optional pattern paramter is supplied, only files that match this regex will be included in the result array.
	 * 
	 * Unlike fs.list(), the file names returned are the full relative path to the file.
	 * 
	 * @param {string} dir - path to directory to get list of files for.
	 * @param {regex} pattern - pattern to filter list of returned files.
	 * @returns {array} files - array of file names.
	 */
    listRecursive: function(dir, pattern) {
        var files = [];
        var recurse = pattern ? function(dir) {
            fs.readDir(dir).each(function(file) {
                if (fs.isDir(dir + '/' + file)) {
                    if (pattern.test(file)) {
                        files.push(dir + '/' + file);
                    }
                    recurse(dir + '/' + file);
                }
                else if (pattern.test(file)) {
                    files.push(dir + '/' + file);
                }
            });
        } : function(dir) {
            var dirEntries = fs.readDir(dir);
            dirEntries && dirEntries.each(function(file) {
                if (fs.isDir(dir + '/' + file)) {
                    files.push(dir + '/' + file);
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
    
	/**
	 * @function fs.removeDirectory
	 * 
	 * ### Synopsis
	 * 
	 * fs.removeDirectory(path);
	 * 
	 * This function recursively removes a directory.  That is, the directory and any files or directories contained within will be deleted.
	 * 
	 * ### Note
	 * 
	 * DANGER!  This function can delete a lot of files and directories and if you're not careful, you may lose something you don't want to delete.
	 * 
	 * @param {string} path - path to directory to remove.
	 */
    removeDirectory: function(dir) {
        function recurse(path) {
//            console.log('recurse ' + path);
            var files = fs.readDir(path);
            if (files) {
                files.each(function(file) {
                    if (fs.isDir(path+'/'+file)) {
                        recurse(path + '/' + file);
                        fs.rmdir(path + '/' + file);
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
    },
    
    /**
     * @function fs.copy
     * 
     * ### Synopsis
     * 
     * fs.copy(dst, src);
     * 
     * @param {string} dst - destination path where file is copied.
     * @param {string} src - path to file to be copied.
     */
    copy: function(dst, src) {
        process.exec('cp ' + src + ' ' + dst);
    },
    
    /**
     * @function fs.mkdir
     * 
     * ### Synopsis
     * 
     * var status = fs.mkdir(path);
     * var status = fs.mkdir(path, parents);
     * 
     * Create a directory.
     * 
     * @param {string} path - path of directory to create.
     * @param {boolean} parents - make parent directories as needed.
     * 
     * ### Note
     * 
     * If parents is not true, then fs.mkdir() will fail if any part of the path does not exist, except the last part (the directory name).
     */
    mkdir: function(path, parents) {
        if (!parents) {
            return fs.mkdir(path);
        }
        else {
            var toCreate = '/',
                ret;
            path.split('/').each(function(part) {
                if (part.length) {
                    toCreate += part;
                    if (!fs.exists(toCreate)) {
                        ret = fs.mkdir(toCreate);
                    }
                    toCreate += '/';
                }
            });
            return ret;
        }
    }
}

exports.extend(fs);
exports.extend(newfs);
