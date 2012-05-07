var fs = require('fs'),
    console = require('console'),
    expat = require('builtin/expat');

function start(el, attr) {
	console.log(el);
//	console.dir(attr);
}

function end(el) {
	console.log(' END ' + el);
}

function charHandler(text) {
    console.log('(text) ' + text);
}

function main() {
	var content = fs.readFile('cd_catalog.xml');
	var parser = expat.parser(this, start, end, charHandler);
	expat.parse(parser, content);
}

