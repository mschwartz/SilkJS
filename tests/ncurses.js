/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

include("lib/print_r.js");
function demo1() {
	ncurses.initscr();
	ncurses.printw('hello world !!!');
	ncurses.refresh();
	ncurses.getch();
	ncurses.endwin();
}

function demo2() {
	ncurses.initscr();			/* Start curses mode 		*/
	ncurses.raw();				/* Line buffering disabled	*/
	ncurses.keypad(ncurses.stdscr, true);		/* We get F1, F2 etc..		*/
	ncurses.noecho();			/* Don't echo() while we do getch */

    ncurses.printw("Type any character to see it in bold\n");
	ch = ncurses.getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
	if(ch == ncurses.KEY_F(1)) {		/* Without keypad enabled this will */
		ncurses.printw("F1 Key pressed");/*  not get to us either	*/
					/* Without noecho() some ugly escape
					 * charachters might have been printed
					 * on screen			*/
	}
	else
	{
		ncurses.printw("The pressed key is ");
		ncurses.attron(ncurses.A_BOLD);
		ncurses.printw(String.fromCharCode(ch));
		ncurses.attroff(ncurses.A_BOLD);
	}
	ncurses.refresh();			/* Print it on to the real screen */
    ncurses.getch();			/* Wait for user input */
	ncurses.endwin();			/* End curses mode		  */
}

function demo3() {
	var mesg = 'just a string';
	ncurses.initscr();
	var maxyx = ncurses.getmaxyx();
	var row = maxyx.row, col = maxyx.col;
	ncurses.mvprintw(row/2, (col-mesg.length)/2, mesg);
	ncurses.mvprintw(row-2, 0, 'this screen has ' + row + ' rows and ' + col + ' columns\n');
	ncurses.printw('try resizing your window (if possible) and then run this program again');
	ncurses.refresh();
	ncurses.getch();
	ncurses.endwin();
}

function demo4() {
	var EOF = 'xxx';
	var prev = EOF;
	try {
		ncurses.initscr();				/* Start curses mode */
		var file = fs.readFile('main.cpp');
		var maxyx = ncurses.getmaxyx();		/* find the boundaries of the screeen */
		var row = maxyx.row, col = maxyx.col;
		var yx, x, y;
		for (var i=0, len=file.length; i<len; i++) {
			var ch = file.charAt(i);
			yx = ncurses.getyx();		/* get the current curser position */
			y = yx.y;
			x = yx.x;
			if(y == (row - 1))			/* are we are at the end of the screen */
			{
				ncurses.printw("<-Press Any Key->");	/* tell the user to press a key */
				ncurses.getch();
				ncurses.clear();				/* clear the screen */
				ncurses.move(0, 0);			/* start at the beginning of the screen */
			}
			if(prev == '/' && ch == '*')    	/* If it is / and * then only
											 * switch bold on */    
			{
				ncurses.attron(ncurses.A_BOLD);			/* cut bold on */
				yx = ncurses.getyx();		/* get the current curser position */
				y = yx.y; x = yx.x;
				ncurses.move(y, x - 1);			/* back up one space */
				ncurses.printw('/' + ch); 		/* The actual printing is done here */
			}
			else
				ncurses.printw(ch);
			ncurses.refresh();
			if(prev == '*' && ch == '/')
				ncurses.attroff(ncurses.A_BOLD);        		/* Switch it off once we got *
										 * and then / */
			prev = ch;
		}
	}
	finally {
		ncurses.getch();
		ncurses.endwin();                       	/* End curses mode */
	}
}

function demo5() {
	var my_win, startx, starty, width, height, ch;
	ncurses.initscr();
	ncurses.cbreak();
	ncurses.keypad(ncurses.stdscr, true);		/* I need that nifty F1 	*/

	height = 3;
	width = 10;
	starty = (ncurses.LINES - height) / 2;	/* Calculating for a center placement */
	startx = (ncurses.COLS - width) / 2;	/* of the window		*/
	ncurses.printw("Press F1 to exit");
	ncurses.refresh();
	my_win = create_newwin(height, width, starty, startx);

	while((ch = ncurses.getch()) != ncurses.KEY_F(1))
	{	switch(ch)
		{	case ncurses.KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case ncurses.KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case ncurses.KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case ncurses.KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;	
		}
	}
		
	ncurses.endwin();			/* End curses mode		  */
	
	function create_newwin(height, width, starty, startx) {
		var local_win = ncurses.newwin(height, width, starty, startx);
		ncurses.box(local_win, 0, 0);
		ncurses.wrefresh(local_win);
		return local_win;
	}
	
	function destroy_win(local_win) {
		ncurses.wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		ncurses.wrefresh(local_win);
		ncurses.delwin(local_win);
	}
}

function main(fn) {
	function clearEol() {
		var xy = ncurses.getyx();
		var max_xy = ncurses.getmaxyx();
		var x = xy.x, maxx = max_xy.x;
		for (var i=x; i<maxx; i++) {
			ncurses.printw(' ');
		}
	}
	
	var entry, alive = true;
	while (alive) {
		ncurses.initscr();
		ncurses.cbreak();
		ncurses.clear();

		var row = 0;
		ncurses.attron(ncurses.A_STANDOUT);
		ncurses.mvprintw(row++, 0, ' SilkJS NCurses Demos');
//		ncurses.clrtoeol();
		clearEol();
		ncurses.attroff(ncurses.A_STANDOUT);
		ncurses.mvprintw(++row, 0, '1) Hello, world');
		ncurses.mvprintw(++row, 0, '2) getch demo');
		ncurses.mvprintw(++row, 0, '3) window size demo');
		ncurses.mvprintw(++row, 0, '4) less demo');
		ncurses.mvprintw(++row, 0, '5) window demo');
		row++;
		ncurses.mvprintw(++row, 0, 'Press # or 0 to exit');
		ncurses.refresh();
		var raw = ncurses.getch();
		if (raw === ncurses.KEY_RESIZE) {
//			ncurses.clear();
			ncurses.endwin();
			continue;
		}
		entry = String.fromCharCode(raw);
		ncurses.clear();
		ncurses.endwin();
		switch (entry) {
			case '0':
			case 'q':
				alive = false;
				break;
			case '1':
				demo1();
				break;
			case '2':
				demo2();
				break;
			case '3':
				demo3();
				break;
			case '4':
				demo4();
				break;
			case '5':
				demo5();
				break;
			
		}
	}
}
