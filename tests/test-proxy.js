var console = require('console');

var p = Proxy.create({
	get: function(receiver, name) {
		console.log(receiver);
		return "something";
	}
});
console.log(p.abc);

