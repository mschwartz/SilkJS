#!/usr/local/bin/silkjs

var process = require('builtin/process'),
	console = require('console');

function main() {
	process.exec('./exit123.js');
	console.log('return code = ' + process.exec_result());
	process.exec('./return123.js');
	console.log('returns = ' + process.exec_result());
}

