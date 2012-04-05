/**
 * @class SFTP
 * 
 * ### Synopsis
 * 
 * SFTP interface for SilkJS
 * 
 * ### Example
 * 
 * ```
 * var SFTP = require('SFTP');
 * var connection = new SFTP(host, user, password);
 * console.log(connection.dir());
 * ```
 */

"use strict";

/*global error */
var sftp = require('builtin/sftp');

function isString(v) {
    return typeof v === 'string';
}

/**
 * @constructor SFTP
 * 
 * ### Synopsis
 * 
 * var sftp = new SFTP(host, username, password);
 * var sftp = new SFTP(host, username, password, port);
 * 
 * Create an SFTP connection to a remote host.
 * 
 * @param {string} host - name of host to connect to.
 * @param {string} username - name of user to log in as at remote host.
 * @param {string} password - password of user to log in as at remote host.
 * @param {int} port - port that SSH daemon is running on at remote host (defaults to 22).
 * @returns {object} sftp - instance of instantiated SFTP connection.
 * 
 * ### Notes
 * 
 * Throws an error if connection does not succeed.
 */
function SFTP(host, username, password, port) {
//    console.log('SFTP CONSTRUCTOR')
//    console.dir(this);
//    console.dir(this.prototype);
    this.handle = sftp.connect(host, username, password, port || 22);
    if (isString(this.handle)) {
        error(this.handle);
    }
    this.connected = true;
//    console.dir(this.prototpye);
}

SFTP.prototype.extend({
    /**
     * @function sftp.close
     * 
     * ### Synopsis
     * 
     * sftp.close();
     * 
     * Closes connection and frees any resources used.
     */
    close: function() {
        sftp.close(this.handle);
    },
    /**
     * @function sftp.readDir
     * 
     * ### Synopsis
     * 
     * var files = sftp.readDir(path);
     * 
     * Reads a directory from remote server, returns an array of objects of the form:
     * 
     * + name: name of file.
     * + longentry: a string?
     * + size: size of file in bytes.
     * + permissions: file permissions.
     * + uid: uid of owner of file.
     * + gid: gid of owner of file.
     * + mtime: modification time (timestamp) of file.
     * + atime: access time (timestamp) of file
     * 
     * The above values may be undefined except for name if the server does not support extended information protocol.
     * 
     * @param {string} path - path to directory on remote server to get listing of.
     * @returns {array} files - array of file objects as described above.
     */
    readDir: function(path) {
        return sftp.readDir(this.handle, path);
    },
    
    /**
     * @function sftp.stat
     * 
     * ### Synopsis
     * 
     * var info = sftp.stat(path);
     * 
     * Get attributes of remote file or directory.
     * 
     * The object returned will be of the form:
     * 
     * + size: size of file in bytes.
     * + permissions: file permissions.
     * + uid: uid of owner of file.
     * + gid: gid of owner of file.
     * + mtime: modification time (timestamp) of file.
     * + atime: access time (timestamp) of file
     * 
     * @param {string} path - path to file or directory to get information about on the remote server.
     * @returns {object} info - object of the form described above.
     */
    stat: function(path) {
        return sftp.stat(this.handle, path);
    },
    
    /**
     * @function sftp.mkdir
     * 
     * ### Synopsis
     * 
     * var success = sftp.mkdir(path);
     * var success = sftp.mkdir(path, mode);
     * 
     * Create a directory on the remote host.
     * 
     * @param {string} path - path of directory to create.
     * @param {int} mode - permissions of resulting directory.
     * @returns {boolean} success - true if directory was created.
     */
    mkdir: function(path, mode) {
        return sftp.mkdir(this.handle, path, mode || parseInt('755', 8));
    },

    /**
     * @function sftp.rmdir
     * 
     * ### Synopsis
     * 
     * var success = sftp.mkdir(path);
     * 
     * Remove a directory on the remote host. The directory must exist and be empty.
     * 
     * @param {string} path - path of directory to remove.
     * @returns {boolean} success - true if directory was removed.
     */
    rmdir: function(path) {
        return sftp.rmdir(this.handle, path);
    },
    
    /**
     * @function sftp.unlink
     * 
     * ### Synopsis
     * 
     * var success = sftp.unlink(path);
     * 
     * Remove a file on the remote host
     * 
     * @param {string} path - path of file to remove.
     * @returns {boolean} success - true if directory was removed.
     */
    unlink: function(path) {
        return sftp.unlink(this.handle, path);
    },
    
    /**
     * @function sftp.writeFile
     * 
     * ### Synopsis
     * var status = sftp.writeFile(srcPath, dstPath);
     * var status = sftp.writeFile(srcPath, dstPath, mode);
     * 
     * Write file to remote server via SFTP.
     * 
     * @param {string} srcPath - path to file in local file system to send.
     * @param {string} dstPath - path to file in remote file system to create.
     * @param {int} mode - desired resulting file permissions of file on remote end.
     * @return {boolean} success - true if the transfer succeeded.
     * 
     * ### Note
     * If mode is not provided, the file mode of the file being sent will be used.
     */
    writeFile: function(srcPath, dstPath, mode) {
        if (mode) {
            return sftp.writeFile(this.handle, srcPath, dstPath, mode);
        }
        return sftp.writeFile(this.handle, srcPath, dstPath);
    }
    
});

exports = SFTP;
