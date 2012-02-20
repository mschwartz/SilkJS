(function() {
	console = builtin.console;
	exports = {
		log: console.log,
		dir: function(o) {
			console.log(print_r(o));
		}
	};
}());
