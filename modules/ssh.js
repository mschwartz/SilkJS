/**
 * @class SSH
 * 
 * ### Synopsis
 * 
 * var SSH = require('ssh');
 * var connection = new SSH(host, user, password);
 * 
 * SilkJS SSH2 integration.
 */

"use strict";

var ssh2 = require('builtin/ssh2');

/**
 * @constructor SSH
 * 
 * ### Synopsis:
 * 
 * var ssh = new SSH(host, user, password);
 * 
 * Create an SSH connection to the specified host using the specified credentials.
 * 
 * @param {string} host - hostname to connect to.
 * @param {string} user - user login at remote host
 * @param {string} password - user password at remote host
 * @returns {object} ssh - ssh connection instance, connected.
 */
var SSH = function(aHost, aUser, aPassword) {
    this.connection = ssh2.connect(aHost, aUser, aPassword);
    if (!ssh2.alive(this.connection)) {
        throw ssh2.error(this.connection);
    }
};
SSH.prototype.extend({
    /**
     * @function SSH.alive
     * 
     * ### Synopsis
     * 
     * var isAlive = ssh.alive();
     * 
     * Determine if connection is alive.
     * 
     * @returns {boolean} isAlive - true if connection is valid
     */
    alive: function() {
        return ssh2.alive(this.connection);
    },
    
    /**
     * @function SSH.error
     * 
     * ### Synopsis
     * 
     * var message = ssh.error();
     * 
     * Get error message for last operation.
     * 
     * @returns {string} message - error message.
     */
    error: function() {
        return ssh2.error(this.connection);
    },
    
    /**
     * @functino SSH.exec
     * 
     * ### Synopsis
     * 
     * var text = ssh.exec(command);
     * 
     * Execute a command on the remote host, return output of that command.
     * 
     * @param {string} command - shell-type command to execute on remote host.
     * @returns {string} text - output of the command, or false if an error occured.
     */
    exec: function(aCommand) {
        if (!ssh2.exec(this.connection, aCommand)) {
            return false;
        }
        return ssh2.response(this.connection);
    },
    
    /**
     * @function SSH.exitCode
     * 
     * ### Synopsis
     * 
     * var code = ssh.exitCode();
     * 
     * Get exit code of last executed command.  This would be the value of exit() called by the program on the remote host.
     * 
     * @returns {int} code - exit code.
     */
    exitCode: function() {
        return ssh2.exit_code(this.connection);
    },
    
    /**
     * @function SSH.close
     * 
     * ### Synopsis
     * 
     * ssh.close();
     * 
     * Close remote connection and free any resources used.
     * 
     */
    close: function() {
        ssh2.close(this.connection);
    },
    
    /**
     * @function SSH.putFile
     * 
     * ### Synopsis
     * 
     * var success = ssh.putFile(localPath, remotePath);
     * var success = ssh.putFile(localPath, remotePath, mode);
     * 
     * Use scp protocol to copy a file from local file system to remote host.
     * 
     * @param {string} localPath - path to file on local file system to be copied.
     * @param {string} remotePath - path to file to create on remote file system.
     * @param {int} mode - permissions for resulting file on remote host (defaults to 644).
     * @return {boolean} success - true if the file was copied.
     */
    putFile: function(localPath, remotePath, mode) {
        return ssh2.writeFile(this.connection, localPath, remotePath, mode || 0644);
    }
});

if (exports) {
    exports = SSH;
}
