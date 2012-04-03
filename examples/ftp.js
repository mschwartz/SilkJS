#!/usr/local/bin/silkjs

var ftp = require('ftp'),
    process = require('builtin/process'),
    ncurses = require('builtin/ncurses'),
    editline = require('builtin/editline'),
    console = require('console');

function main(host, username, password) {
    if (!host) {
        println("Usage: ftp.js host [username [password]]");
        return;
    }
//    ncurses.initscr();
    if (!username) {
        username = process.getlogin();
    }
    if (!password) {
        password = console.getPassword('Enter password for ' + username + ': ');
    }
//    ncurses.endwin();
   
    var handle = editline.init();
    var conn = new ftp(host, username, password);
    console.log(conn.cwd);
    console.log(conn.dir().trim());
    editline.end(handle);
}
