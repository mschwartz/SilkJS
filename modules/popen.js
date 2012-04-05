/**
 * @class popen
 * 
 * ### Synopsis
 * 
 * Interface to popen()
 */

"use strict";

var console = require('console'),
    POPEN = require('builtin/popen');

var popen = function(cmd, type) {
    this.fd = POPEN.open(cmd, type);
}
popen.prototype.extend({
    gets: function(size) {
        return size ? POPEN.gets(this.fd, size) : POPEN.gets(this.fd);
    },
    puts: function(s) {
        return POPEN.puts(this.fd, s);
    },
    putln: function(s) {
        return POPEN.puts(this.fd, s + '\r\n');
    },
    close: function() {
        POPEN.close(this.fd);
    }
});

exports.popen = popen;
