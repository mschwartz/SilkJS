fs = require('fs'),
console = require('console');

function main() {
	if (!fs.touch('foo')) {
		console.log(fs.error());
	}
}

