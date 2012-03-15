#!/usr/local/bin/silkjs
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

include('lib/phpjs.js');

var imports = [ 
	'move', 
	'getmaxyx', 
	'getyx', 
	'attron', 
	'attroff', 
	'printw', 
	'mvprintw', 
	'A_BOLD', 
	'A_STANDOUT', 
	'clrtoeol', 
	'initscr', 
	'cbreak', 
	'clear', 
	'refresh', 
	'endwin' ,
	'halfdelay',
	'noecho',
	'getch',
	'ERR'
];
ncurses = require('builtin/ncurses');
fs = require('builtin/fs');
process = require('builtin/process');
v8 = require('builtin/v8');

imports.each(function(value) {
	global[value] = ncurses[value];
});

var sleep = 1;
var compress = 0;
var disk = {};
var current = {}, initial = {}, stat = {};
var hostname, release;
var rows = 0, cols = 0;
var row = 0;

function ResizeWindow() {
	var yx = getmaxyx();
	rows = yx.row;
	cols = yx.col;
}

function Print(row, col, prompt, value) {
	move(row, col);
	attron(A_BOLD);
	printw(prompt);
	attroff(A_BOLD);
	printw(value);
}

function PrintLn(row, col, prompt, value) {
	Print(row, col, prompt, value);
	clrtoeol();
}

function GetStat() {
	var ret = {};
	var lines = fs.readFile('/proc/stat').split('\n');
	lines.each(function(line) {
		var vals = line.split(/\s+/);
		var key = vals.shift();
		ret[key] = vals.join(' ');
	});
	return ret;
}

function InitState() {
	ResizeWindow();
	current = {};
	initial = {};
	stat = GetStat();
	stat.each(function(value, key) {
		current[key] = value.split(/\s+/).join(':::');
		initial[key] = value.split(/\s+/).join(':::');
	});
	hostname = fs.readFile('/proc/sys/kernel/hostname').replace(/\n/igm, '');
	release = fs.readFile('/proc/sys/kernel/osrelease').replace(/\n/igm, '');
}

function clearEol() {
	var xy = getyx();
	var max_xy = getmaxyx();
	var x = xy.x, maxx = max_xy.x;
	for (var i=x; i<maxx; i++) {
		printw(' ');
	}
}

function PrintHeader() {
	row = 0;
	attron(A_STANDOUT);
	mvprintw(row++, 0, ' systat/'+sleep+ ' [' + hostname + '/' + release + '] ' + new Date().toString());
	clearEol();
	attroff(A_STANDOUT);
}

function PrintLoadAverage() {
	var lines = fs.readFile('/proc/loadavg').split('\n');
	var line = lines[0];
	var parts = line.split(/\s+/);
	var processes = parts[3].split('/');
	move(row++, 0);
	attron(A_BOLD); printw('Load Average:'); attroff(A_BOLD);
	printw(' ' + parts[0] + ' ' + parts[1] + ' ' + parts[2] + ' ');
	attron(A_BOLD); printw('Processes:'); attroff(A_BOLD);
	printw(' ' + processes[1] + ' ');
	attron(A_BOLD); printw('Running:'); attroff(A_BOLD);
	printw(' ' + processes[0]);
	clearEol();
}

function PrintCPU() {
	if (!compress) {
		row++;
	}
	move(row++, 0);
	attron(A_BOLD); attron(A_STANDOUT);
	printw( "CPU STATES             Current                   Aggregate         ");
	move(row++, 0);
	printw("              user    sys   nice   idle    user   sys  nice   idle ");
	attroff(A_BOLD); attroff(A_STANDOUT);
	stat.each(function(value, key) {
		if (key.indexOf('cpu') == 0) {
			var prompt = key.toUpperCase();
			var parts = stat[key].split(/\s+/);
			var currentUser = parseInt(parts[0], 10), 
				currentNice = parseInt(parts[1], 10), 
				currentSystem = parseInt(parts[2], 10), 
				currentIdle = parseInt(parts[3], 10);
			
			parts = current[key].split(':::');
			var lastUser = parseInt(parts[0], 10), 
				lastNice = parseInt(parts[1], 10), 
				lastSystem = parseInt(parts[2], 10), 
				lastIdle = parseInt(parts[3], 10);
			var deltaUser = currentUser - lastUser;
			var deltaNice = currentNice - lastNice;
			var deltaSystem = currentSystem - lastSystem;
			var deltaIdle = currentIdle - lastIdle;
			current[key] = currentUser + ':::' + currentNice + ':::' + currentSystem + ':::' + currentIdle;
			var total = deltaUser + deltaNice + deltaSystem + deltaIdle;
			if (total) {
				zuser = sprintf('%5.1f', parseInt(deltaUser/total*1000+.5, 10)/10);
				znice = sprintf('%5.1f', parseInt(deltaNice/total*1000+.5, 10)/10);
				zsystem = sprintf('%5.1f', parseInt(deltaSystem/total*1000+.5, 10)/10);
				zidle = sprintf('%5.1f', parseInt(deltaIdle/total*1000+.5, 10)/10);
			}
			else {
				zuser = znice = zsystem = zidle = sprintf('%5.1f', 0.0);
			}
//zuser = yuser; zsystem = ysystem; znice = ynice; zidle = yidle;
			move(row++, 0);
			attron(A_BOLD);
			printw(sprintf('%-12.12s', prompt));
			attroff(A_BOLD);
			printw(zuser + '% ' + zsystem + '% ' + znice + '% ' + zidle + '%');
			parts = initial[key].split(':::');
			lastUser = parseInt(parts[0], 10); 
			lastNice = parseInt(parts[1], 10); 
			lastSystem = parseInt(parts[2], 10); 
			lastIdle = parseInt(parts[3], 10);
			deltaUser = currentUser - lastUser;
			deltaNice = currentNice - lastNice;
			deltaSystem = currentSystem - lastSystem;
			deltaIdle = currentIdle - lastIdle;
			total = deltaUser + deltaNice + deltaSystem + deltaIdle;
			if (total) {
				zuser = sprintf('%4.1f', parseInt(deltaUser/total*1000+.5, 10)/10);
				znice = sprintf('%4.1f', parseInt(deltaNice/total*1000+.5, 10)/10);
				zsystem = sprintf('%4.1f', parseInt(deltaSystem/total*1000+.5, 10)/10);
				zidle = sprintf('%5.1f', parseInt(deltaIdle/total*1000+.5, 10)/10);
			}
			else {
				zuser = znice = zsystem = zidle = sprintf('%5.1f', 0.0);
			}
			printw('   ' + zuser + '% ' + zsystem + '% ' + znice + '% ' + zidle + '%');
			clearEol();
		}
	});
}

function PrintMemory() {
	if (!compress) {
		row++;
	}
	var memInfo = {};
	var lines = fs.readFile('/proc/meminfo').split('\n');
	lines.each(function(line) {
		line = line.replace(/\s+/g, '');
		var parts = line.split(':');
		memInfo[parts[0]] = parseInt(parts[1], 10);
	});
	memInfo.MemUsed = memInfo.MemTotal - memInfo.MemFree;
	memInfo.MemAvail = memInfo.MemFree + memInfo.Cached + memInfo.Buffers;
	memInfo.SwapUsed = memInfo.SwapTotal - memInfo.SwapFree;
	attron(A_BOLD); attron(A_STANDOUT);
	move(row++, 0);
	printw(sprintf("%-10.10s  %8.8s %8.8s %8.8s %8.8s %8.8s %8.8s %8.8s",
         "MEMORY", "Total", "Used", "Free", "Shared", "Buffer", "Cache", "Avail"));
	attroff(A_BOLD); attroff(A_STANDOUT);
	PrintLn(row++, 0, sprintf('%-10.10s  ', 'Real'), sprintf('%8.8s %8.8s %8.8s %8.8s %8.8s %8.8s %8.8s',
		memInfo.MemTotal, memInfo.MemUsed, memInfo.MemFree, 0, memInfo.Buffers, memInfo.Cached, memInfo.MemAvail));
	PrintLn(row++,0, sprintf("%-10.10s  ", "Swap"), sprintf("%8.8s %8.8s %8.8s", 
			memInfo.SwapTotal,
			memInfo.SwapUsed,
			memInfo.SwapFree
	));	
	clearEol();
}

var disk_reads = {};
var disk_writes = {};
function PrintDisk() {
	if (!compress) {
		row++;
	}
	attron(A_BOLD); attron(A_STANDOUT);
	move(row++, 0);
	printw('DISK ACTIVITY           Current             Aggregate  ');
	move(row++, 0);
	printw(' (K bytes)            IN       OUT        IN       OUT ');
	attroff(A_BOLD); attroff(A_STANDOUT);
	
	var lines = fs.readFile('/proc/diskstats').split('\n');
	lines.each(function(line) {
		line = line.replace(/^\s+/g, '');
		var arr = line.split(/\s+/);
		var drive = arr[2];
		if (!drive || drive.search(/\d$/) !== -1 || arr[3] == 0) { //} || drive.search(/\d$/)) { //} || arr[3] == 0) {
			return;
		}
		move(row++, 0);
		attron(A_BOLD);
		printw(sprintf("%-16.16s", '/dev/' + drive));
		attroff(A_BOLD);
		
		disk_reads[drive] = disk_reads[drive] || 0;
		var reads = parseInt(arr[5], 10);
		var deltaReads = reads - disk_reads[drive];
		disk_reads[drive] = reads;
		
		disk_writes[drive] = disk_writes[drive] || 0;
		var writes = parseInt(arr[9], 10);
		var deltaWrites = writes - disk_writes[drive];
		disk_writes[drive] = writes;
		printw(sprintf('%8d  %8d  %8d  %8d', deltaReads*512/1024, deltaWrites*512/1024, reads*512/1024, writes*512/1024));
		clearEol();
	});
}

var lastpgin = 0, lastpgout = 0, lastswpin = 0, lastswpout = 0;
function PrintVirtualMemory() {
	if (!compress) {
		row++;
	}
	
	attron(A_BOLD); attron(A_STANDOUT);
	move(row++, 0);
	printw('VIRTUAL MEMORY     Current          Aggregate  ');
	move(row++, 0);
	printw('                  IN   OUT        IN       OUT ');
	attroff(A_BOLD); attroff(A_STANDOUT);

	var lines = fs.readFile('/proc/vmstat').split('\n');
	var vmstat = {};
	lines.each(function(line) {
		var parts = line.split(/\s+/);
		vmstat[parts[0]] = parseInt(parts[1], 10);
	});
	if (!lastpgin) {
		lastpgin = vmstat.pgpgin;
		lastpgout = vmstat.pgpgout;
		lastswpin = vmstat.pgswpin;
		lastswpout = vmstat.pgswpout;
	}
	
	move(row++, 0);
	attron(A_BOLD);
	printw(sprintf("%-16.16s", 'page'));
	attroff(A_BOLD);
	var pgin = vmstat.pgpgin;
	var deltaIn = pgin - lastpgin;
	lastpgin = pgin;
	var pgout = vmstat.pgpgout;
	var deltaOut = pgout - lastpgout;
	lastpgout = pgout;
	printw(sprintf('%4d  %4d  %8d  %8d', deltaIn, deltaOut, pgin, pgout));
	clearEol();
	
	move(row++, 0);
	attron(A_BOLD);
	printw(sprintf("%-16.16s", 'swap'));
	attroff(A_BOLD);
	var swpin = vmstat.pswpin;
	deltaIn = swpin - lastswpin;
	lastswpin = swpin;
	var swpout = vmstat.pgswpout;
	deltaOut = swpout - lastswpout;
	lastswpout = swpout;
	printw(sprintf('%4d  %4d  %8d  %8d', deltaIn, deltaOut, swpin, swpout));
	clearEol();
}

function main() {
	var done = false;
	InitState();
	try {
		initscr();
		cbreak();
//		halfdelay(sleep*10);
		noecho();
		clear();
		while (!done) {
			halfdelay(sleep*2);
			var c = getch();
			if (c != ERR) {
				c = String.fromCharCode(c);
				switch (c) {
					case 'c':
						clear();
						compress = !compress;
						break;
					case 'q':
						done = true;
						break;
					case '1':
						sleep = 1;
						break;
					case '2':
						sleep = 2;
						break;
					case '3':
						sleep = 3;
						break;
					case '4':
						sleep = 4;
						break;
					case '5':
						sleep = 5;
						break;
				}
			}
			if (done) {
				break;
			}
			ResizeWindow();
			stat = GetStat();
			PrintHeader();
			PrintLoadAverage();
			PrintCPU();
			PrintMemory();
			PrintDisk();
			PrintVirtualMemory();
			move(0,cols-1);
			refresh();
			v8.gc();
			process.sleep(sleep);
		}
	}
	finally {
		endwin();
	}
}
