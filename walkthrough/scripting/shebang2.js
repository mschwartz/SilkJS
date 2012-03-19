#!/usr/local/bin/silkjs

function main(name) {
	if (!name) {
		println('Usage: ./shebang2.js name');
		println('name is required!');
	}
	else {
		println('Hi there, ' + name + '.');
	}
}

