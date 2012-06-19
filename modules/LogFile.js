/**
 * @class LogFile
 *
 * ### Synopsis
 *
 * var logfile = require('LogFile');
 *
 * ### Description
 *
 * JavaScript implementation of logfile class.
 *
 * ### Notes
 *
 * LogFiles should be created before calling process.fork() if the child processes are to share the log file.
 *
 * The log file will be flushed upon calling the flush() method, or the destroy() method.  The constructor forks a process to periodically flush the buffered log file lines to disk.
 */
/*global require, exports: true, log */

(function() {
    "use strict";
    var logfile = require('builtin/logfile'),
        process = require('builtin/process');
    /**
     * @constructor LogFile
     *
     * ### Synopsis
     *
     * var logfile = new LogFile(filename);
     *
     * Construct a new LogFile object.  Once constructed, caller may fork() and all children may write to the logfile at will.
     *
     * @param {string} filename - name of the log file on disk.
     * @returns {object} logfile - instance of LogFile class.
     */
    var LogFile = function(filename) {
        var me = this;
        this.handle = logfile.init(filename);
        this.pid = process.fork();
        if (!this.pid) {
            // child
            while (true) {
                process.sleep(5);
                logfile.flush(me.handle);
            }
        }
        else {
            log('Started logfile process ' + this.pid);
        }
    };
    LogFile.prototype.extend({
        /**
         * @function LogFile.write
         *
         * ### Synopsis:
         *
         * logfile.write(s);
         * logfile.write(s, len);
         *
         * Write a string to the log file.
         *
         * @param {string} s - string to write.
         * @param {int} len - length of string to write (optional).  If omitted, the full string will be written.
         */
        write: function(s, len) {
            return len ? logfile.write(this.handle, s, len) : logfile.write(this.handle, s);
        },
        /**
         * @function LogFile.writeln
         *
         * ### Synopsis:
         *
         * logfile.writeln(s);
         *
         * Write a string to the log file with an appended newline.
         *
         * @param {string} s - string to write.
         */
        writeln: function(s, len) {
            return logfile.write(this.handle, s + '\n');
        },
        /**
         * @function LogFile.flush
         *
         * ### Synopsis
         *
         * logfile.flush();
         *
         * Flushes contents of buffered log file lines to disk.
         */
        flush: function() {
            return logfile.flush(this.handle);
        },
        /**
         * @function LogFile.destroy
         *
         * ### Synopsis
         *
         * logfile.destroy();
         *
         * Free all memory and resources used by the LogFile instance.
         */
        destroy: function() {
            try {
                process.kill(this.pid);
            }
            catch (e) {
            }
            logfile.flush(this.handle);
            return logfile.destroy(this.handle);
        }
    });
    exports = LogFile;
}());