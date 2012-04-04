#!/usr/local/bin/silkjs

/* 
 * Sample program to test ssh exec
 */
var SSH = require('SSH');

function main(hostname, username, password) {
    var console = require('builtin/console');
	var con = new SSH(hostname, username, password);
	console.log(con.exec('ls -l src/SilkJS/modules'));
	con.close();
}

