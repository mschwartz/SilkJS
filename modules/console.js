(function() {
	console = builtin.console;
	print_r = require('builtin/print_r');
	exports = {
		log: console.log,
		dir: function(o) {
			console.log(print_r(o));
		}
	};
}());
