(function() {
	console = builtin.console;
	print_r = require('print_r');
	exports = {
		log: console.log,
		dir: function(o) {
			console.log(print_r(o));
		}
	};
}());
