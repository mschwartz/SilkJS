#!/usr/local/bin/silkjs

/* 
 * Sample program to test ssh exec
 */
include('lib/ssh.js');

function main(hostname, username, password) {
	var con = new SSH(hostname, username, password);
	console.log(con.exec('ls -l'));
	con.close();
}

