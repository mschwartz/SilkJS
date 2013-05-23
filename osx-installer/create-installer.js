#!/usr/local/bin/silkjs

var NASM = 'nasm-2.10.07';

var fs = require('fs'),
    getcwd = fs.getcwd,
    chdir  = fs.chdir,
    process = require('builtin/process'),
    exec = process.exec,
    console = require('console'),
    print = console.log;

var cwd = getcwd();

print('Creating tar archive');
exec('tar -cz -C/usr/local -fsilkjs.tgz --exclude contrib/* silkjs bin/httpd-silk.js bin/silkjs');

if (!fs.exists('nasm')) {
    print('Extracting nasm source...');
    exec('tar xzfp ' + NASM + '.tar.gz');
    chdir(NASM);
    print('Configuring nasm...')
    exec('./configure');
    print('Building nasm...');
    exec('make');
    exec('cp nasm ..');
    chdir('..');
}

exec('./nasm -fmacho64 tarfile.s -o tarfile.o');
exec('gcc -o silkjs-installer-osx installer.cpp tarfile.o');
// fs.chdir('/usr/local');

// console.dir(fs.getcwd());
