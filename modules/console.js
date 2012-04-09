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
		 * @function console.error
		 * 
		 * ### Synopsis
		 * 
		 * console.error(str);
		 * 
		 * Write a string to the stderr.
		 * 
		 * @param {string} str - the string to write
		 */
		error: console.error,
        
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
            var save = Error.prepareStackTrace;
            Error.prepareStackTrace = function(error, structuredStackTrace) {
//log('prepareStackTrace');
                var stack = [];
                structuredStackTrace.each(function(item) {
                    stack.push({
                        fileName: item.getFileName(),
                        lineNumber: item.getLineNumber(),
                        functionName: item.getFunctionName()
                    });
                });
                return stack;
            }
            var e = new Error();
            var callee = e.stack[1];
            Error.prepareStackTrace = save;
            callee.functionName = callee.functionName ? (' (' + callee.functionName + ')') : '';
			console.log(callee.fileName + ' line ' + callee.lineNumber + callee.functionName + ':\n ' + print_r(o));
		},

        /**
        * @function console.getPassword
        * 
        * var password = console.getPassword(prompt);
        * 
        * ### Synopsis
        * 
        * Display a prompt and readin a password without echoing the characters to the display.
        * 
        * @param {string} prompt - prompt on the line to get the password
        * @returns {string} password - the password entered by the user
        */
       getPassword: function(prompt) {
           prompt = prompt || '';
           return console.getPassword(prompt);
       }
        
	};
}());
