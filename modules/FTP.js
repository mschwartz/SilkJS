/**
 * @module FTP
 * 
 * ### Synopsis
 * 
 * var FTP = require('FTP');
 *
 * FTP interface for SilkJS.
 *
 * ### Example
 * ```
 * try {
 *      var ftp = new FTP(host, username, password);
 *      console.log(ftp.dir());
 *      ftp.close();
 * }
 * catch(e) {
 *      console.log('*** Authentication failed.');
 * }
 * ```
 */

var _ftp = require('builtin/ftp'),
    fs = require('fs'),
    uuid = require('phpjs').uuid;

//function uuid() {
//    function S4() {
//        return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
//    }
//    return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
//}

// parse a directory listing line
function parseLine(line) {
    var parts = line.split(/\s+/);
    return {
        permissions: parts[0],
        directoryCount: parts[1],
        owner: parts[2],
        group: parts[3],
        size: parts[4],
        date: parts[5] + ' ' + parts[6] + ' ' + parts[7],
        name: parts.slice(8).join(' ')
    };

}
/**
 * @constructor FTP
 *
 * ### Synopsis
 *
 * var ftp = new FTP(host, username, password);
 *
 * Creates a new FTP connection.
 *
 * @param {string} host - remote host to connect to.
 * @param {string} username - username on remote host to login as.
 * @param {string} password - password for username on remote host.
 *
 * ### Notes
 *
 * This constructor throws an error if the connection or authentication fails.
 */
function FTP(host, username, password) {
    _ftp.init();
    this.handle = _ftp.connect(host+':21');
    if (!this.handle) {
        error('connect failed');
    }
    var loggedIn = _ftp.login(this.handle, username, password);
    if (!loggedIn) {
        error('login failed');
    }
    this.cwd = _ftp.getcwd(this.handle);
    this.systemType = _ftp.systemType(this.handle);
}
FTP.prototype.extend({
    /**
     * @function ftp.dir
     * 
     * ### Synopsis
     *
     * var listing = ftp.dir(path);
     *
     * Get a raw text listing of remote directory.
     *
     * @param {string} path - path on remote server to get listing of; this path may contain wildcards (e.g. *.zip).
     * @returns {string} listing - raw directory listing, suitable for printing in a terminal window.
     */
    dir: function(path) {
        path = path || this.cwd;
        var tmpFile = '/tmp/' + uuid();
        _ftp.dir(this.handle, path, tmpFile);
        var dir = fs.readFile(tmpFile);
        fs.unlink(tmpFile);
        return dir;
    },

    /**
     * @function ftp.list
     *
     * ### Synopsis
     *
     * var files = fs.list();
     * var files = fs.list(dir);
     * var files = fs.list(dir, pattern);
     *
     * List a remote directory, returning an array of file or directory names.
     *
     * If no directory path is passed, the current directory on the remote server will be listed.
     *
     * If a directory path is passed, it may contain wildcards (e.g. *.zip).
     *
     * If a pattern is passed, only filenames that match the regex will be returned.
     *
     * @param {string} dir - directory path on remote server to get list of files of.
     * @param {regex} pattern - pattern to filter list of returned files.
     * @return {array} files - array of file names and/or directory names in the remote directory.
     */
    list: function(dir, pattern) {
        var listing = this.dir(dir);
        var files = [];
        var lines = listing.split('\n');
        lines.each(function(line) {
            if (!line.trim().length) {
                return;
            }
            var info = parseLine(line);
            if (pattern) {
                if (pattern.test(info.name)) {
                    files.push(info.name);
                }
            }
            else {
                files.push(info.name);
            }
        });
        return files;
    },

    /**
     * @function ftp.stat
     *
     * ### Synopsis
     *
     * var info = ftp.stat(path);
     *
     * Get information about a remote file or directory.
     *
     * @param {string} path - path to remote file or directory
     * @param {object} info - information about file or directory.
     */
    stat: function(path) {
        var parts = path.split('/');
        var name = parts.pop();
        var opath = parts.join('/');
        if (opath == '' && path[0] == '/') {
            opath = '/';
        }
        var listing = this.dir(opath);
        var lines = listing.split('\n');
        var found = null;
        lines.each(function(line) {
            if (!line.trim().length) {
                return;
            }
            var info = parseLine(line);
            if (info.name === name) {
                found = info;
                found.name = path;
                return false;
            }
        });
        return found;
    },

    /**
     * @function ftp.lastResponse
     *
     * ### Synopsis
     *
     * var response = ftp.lastResponse();
     *
     * Returns a pointer to the last response string sent by the server. This can be parsed by the user program to determine more information about the last request or can be displayed along with an error message.
     *
     * @return {string} response - last response string or NULL if handle is not valid.
     */
    lastResponse: function() {
        return _ftp.lastResponse(this.handle);
    },

    /**
     * @function ftp.size
     *
     * var size = ftp.size(path);
     * var size = ftp.size(path, mode);
     *
     * Determine size of remote file.
     *
     * Some servers may not support the SIZE command.
     *
     * If this request fails, the size may be available in the response to a RETR (ftp.open() with type=ftp.FILE_READ).
     *
     * @param {string} path - path to file to get size of.
     * @param {int} mode - either ftp.ASCII or ftp.BINARY.
     * @return {int} size - size of remote file or false if an error occurred.
     */
    size: function(path, mode) {
        return _ftp.size(this.handle, path, mode || _ftp.BINARY);
    },

    /**
     * @function ftp.fileModified
     *
     * var size = ftp.fileModified(path);
     *
     * Determine last modification time of remote file.  The date and time are returned as a string in the format 'YYYYMMDDHHMMSS'
     *
     * Some servers may not support the MDTM command.
     *
     * @param {string} path - path to file to get modification time of.
     * @return {int} size - size of remote file or false if an error occurred.
     */
    fileModified: function(path) {
        return _ftp.fileModified(this.handle, path);
    },

    /**
     * @function ftp.close
     *
     * ### Synopsis
     *
     * ftp.close();
     *
     * Closes the connection to the remote server and frees any resources associated with the connection.
     */
    close: function() {
        _ftp.quit(this.handle);
    },

    /**
     * @function ftp.pasv
     *
     * var success = ftp.pasv();
     * var success = ftp.passive();
     *
     * Set connection to PASV mode.
     *
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    pasv: function() {
        return _ftp.pasv(this.handle);
    },

    passive: function() {
        return _ftp.pasv(this.handle);
    },

    /**
     * @function ftp.active
     *
     * var success = ftp.active();
     *
     * Set connection to ACTIVE mode.
     *
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    active: function() {
        return _ftp.active(this.handle);
    },

    /**
     * @function ftp.chdir
     *
     * var success = ftp.chdir(path);
     *
     * Sends a change working directory request to the server using the specified path.
     *
     * @param {string} path - path of new desired working directory.
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    chdir: function(path) {
        var ret = _ftp.chdir(this.handle, path);
        this.cwd = _ftp.getcwd(this.handle);
        return ret;
    },

    /**
     * @function ftp.mkdir
     *
     * var success = ftp.mkdir(path);
     *
     * Sends a make directory request to the server using the specified path.
     *
     * @param {string} path - path of new desired directory.
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    mkdir: function(path) {
        return _ftp.mkdir(this.handle, path);
    },

    /**
     * @function ftp.mkdir
     *
     * var success = ftp.mkdir(path);
     *
     * Sends a make directory request to the server using the specified path.
     *
     * @param {string} path - path of new desired directory.
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    rmdir: function(path) {
        return _ftp.rmdir(this.handle, path);
    },

    /**
     * @function ftp.getFile
     *
     * ### Synopsis
     *
     * var success = ftp.getFile(remotePath, localPath, mode);
     * var success = ftp.getFile(remotePath, localPath);
     *
     * Retrieve a file from the remote system to a local file.  The second form will do a BINARY mode file transfer.
     *
     * @param {string} remotePath - path to file on remote system to retrieve.
     * @param {string} localPath - path to file on local file system where file will be stored..
     * @param {int} mode - either ftp.ASCII or ftp.BINARY.
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    getFile: function(remotePath, localPath, mode) {
        return _ftp.get(this.handle, remotePath, localPath, mode || _ftp.BINARY);
    },

    /**
     * @function ftp.putFile
     *
     * ### Synopsis
     *
     * var success = ftp.putFile(localPath, remotePath, mode);
     * var success = ftp.putFile(localPath, remotePath);
     *
     * Send a file to the remote system.  The second form will do a BINARY mode file transfer.
     *
     * @param {string} localPath - path to file on local file system that will be sent to remote server.
     * @param {string} remotePath - path to file on remote system to store.
     * @param {int} mode - either ftp.ASCII or ftp.BINARY.
     * @return {int} success - 1 if successful, 0 if an error occurred.
     */
    putFile: function(localPath, remotePath, mode) {
        return _ftp.put(this.handle, localPath, remotePath, mode || _ftp.BINARY);
    },

    /**
     * @function ftp.readFile
     *
     * ### Synopsis
     *
     * var contents = ftp.readFile(remotePath);
     *
     * Read contents of remote file into a string.
     *
     * @param {string} remotePath - path of remote file to read
     * @returns {string} contents - contents of remote file or false on error.
     */
    readFile: function(remotePath) {
        var tmpFile = '/tmp/' + uuid();
        if (!_ftp.get(this.handle, remotePath, tmpFile, _ftp.BINARY)) {
            fs.unlink(tmpFile);
            return false;
        }
        var contents = fs.readFile(tmpFile);
        fs.unlink(tmpFile);
        return contents;
    },
    
    /**
    * @function ftp.unlink
    * 
    * var success = ftp.unlink(path);
    * 
    * Requests that the server remove the specified file from the remote file system.
    * 
    * @param {string} path - path of remote file to remove.
    * @return {int} success - 1 if successful, 0 if an error occurred.
    */
    unlink: function(remotePath) {
        return _ftp.unlink(this.handle, remotePath);
    },
    
    /**
    * @function ftp.rename
    * 
    * ### Synopsis
    * 
    * var success = ftp.rename(oldPath, newPath);
    * 
    * Sends a rename request to the remote server.
    * 
    * @param {string} oldPath - path of existing remote file to be renamed.
    * @param {string} newPath - path to rename existing file to.
    * @return {int} success - 1 if successful, 0 if an error occurred.
    */
    rename: function(oldPath, newPath) {
        return _ftp.rename(this.handle, oldPath, newPath);
    }
});
exports = FTP;
