/** @ignore */
(function() {
	var console = builtin.console;
	print_r = require('builtin/print_r');
	exports = {
		log: console.log,
		dir: function(o) {
            Error.captureStackTrace(this, this.dir);
            var callee = this.stack[0];
			console.log(callee.fileName + ' line ' + callee.lineNumber + ' (' + callee.functionName + ')\n ' + print_r(o));
		}
	};
}());
