/**
 * @module ftp
 * 
 * ### Synopsis
 * 
 * var ftp = require('ftp');
 * var conn = new ftp(host);
 * conn.whatever();
 */

var FTP = require('builtin/ftp'),
    fs = require('fs');

function ftp(host, username, password) {
    FTP.init();
    this.handle = FTP.connect(host+':21');
    if (!this.handle) {
        error('connect failed');
    }
    var loggedIn = FTP.login(this.handle, username, password);
    if (!loggedIn) {
        error('login failed');
    }
    this.cwd = FTP.getcwd(this.handle);
}
ftp.prototype.extend({
    dir: function(path) {
        path = path || this.cwd;
        FTP.dir(this.handle, path, "/tmp/ftp_dir");
        var dir = fs.readFile('/tmp/ftp_dir');
        fs.unlink('/tmp/ftp_dir');
        return dir;
    }
});
exports = ftp;
