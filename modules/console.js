/**
 * @module console
 * 
 * ### Synopsis
 * 
 * var console = require('console');
 * 
 * JavaScript extension of the builtin/console object.
 * 
 * ### Notes
 * 
 * This implementation is somewhat consistent with other server-side implementations.
 * 
 */
(function() {
	var console = builtin.console;
	print_r = require('builtin/print_r');
	exports = {
		/**
		 * @function console.log
		 * 
		 * ### Synopsis
		 * 
		 * console.log(str);
		 * 
		 * Write a string to the console.
		 * 
		 * @param {string} str - the string to write
		 */
		log: console.log,
		/**
		 * @function console.dir
		 * 
		 * ### Synopsis
		 * 
		 * console.dir(o);
		 * 
		 * Dump a JavaScript primitive (Array, Object, string, etc.) to the console.
		 * 
		 * @param {array|object|string|number|function} o - the thing to dump.
		 */
		dir: function(o) {
            Error.captureStackTrace(this, this.dir);
            var callee = this.stack[0];
			console.log(callee.fileName + ' line ' + callee.lineNumber + ' (' + callee.functionName + ')\n ' + print_r(o));
		}
	};
}());
