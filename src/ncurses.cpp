// see http://invisible-island.net/ncurses/man/ncurses.3x.html

#include "SilkJS.h"
#include <ncurses.h>

// see man initscr
static JSVAL ncurses_initscr(JSARGS args) {
	HandleScope scope;
	return scope.Close(External::New(initscr()));
}
static JSVAL ncurses_endwin(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(endwin()));
}
static JSVAL ncurses_isendwin(JSARGS args) {
	HandleScope scope;
	return scope.Close(Boolean::New(isendwin()));
}
static JSVAL ncurses_newterm(JSARGS args) {
	HandleScope scope;
    String::Utf8Value type(args[0]->ToString());
	Local<External>wrap = Local<External>::Cast(args[1]);
	FILE *outfd = (FILE *)wrap->Value();
	wrap = Local<External>::Cast(args[2]);
	FILE *infd = (FILE *)wrap->Value();
	return scope.Close(External::New(newterm(*type, outfd, infd)));
}
static JSVAL ncurses_set_term(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SCREEN *screen = (SCREEN *)wrap->Value();
	return scope.Close(External::New(set_term(screen)));
}
static JSVAL ncurses_delscreen(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SCREEN *screen = (SCREEN *)wrap->Value();
	delscreen(screen);
	return Undefined();
}

// see man cbreak
static JSVAL ncurses_cbreak(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(cbreak()));
}
static JSVAL ncurses_nocbreak(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(nocbreak()));
}
static JSVAL ncurses_echo(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(echo()));
}
static JSVAL ncurses_noecho(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(noecho()));
}
static JSVAL ncurses_halfdelay(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(halfdelay(args[0]->IntegerValue())));
}
static JSVAL ncurses_intrflush(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(intrflush(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_keypad(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(keypad(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_meta(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(meta(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_nodelay(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(nodelay(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_raw(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(raw()));
}
static JSVAL ncurses_noraw(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(noraw()));
}
static JSVAL ncurses_noqiflush(JSARGS args) {
	HandleScope scope;
	noqiflush();
	return Undefined();
}
static JSVAL ncurses_qiflush(JSARGS args) {
	HandleScope scope;
	qiflush();
	return Undefined();
}
static JSVAL ncurses_notimeout(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(nodelay(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_timeout(JSARGS args) {
	HandleScope scope;
	timeout(args[0]->IntegerValue());
	return Undefined();
}
static JSVAL ncurses_wtimeout(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	wtimeout(window, args[1]->IntegerValue());
	return Undefined();
}
static JSVAL ncurses_typeahead(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(typeahead(args[0]->IntegerValue())));
}

// see man attroff
static JSVAL ncurses_attroff(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attroff(args[0]->IntegerValue())));
}
static JSVAL ncurses_wattroff(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattroff(window, args[1]->IntegerValue())));
}
static JSVAL ncurses_attron(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attron(args[0]->IntegerValue())));
}
static JSVAL ncurses_wattron(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattron(window, args[1]->IntegerValue())));
}
static JSVAL ncurses_attrset(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attrset(args[0]->IntegerValue())));
}
static JSVAL ncurses_wattrset(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattrset(window, args[1]->IntegerValue())));
}
static JSVAL ncurses_color_set(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(color_set(args[0]->IntegerValue(), NULL)));
}
static JSVAL ncurses_wcolor_set(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wcolor_set(window, args[1]->IntegerValue(), NULL)));
}
static JSVAL ncurses_standend(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(standend()));
}
static JSVAL ncurses_wstandend(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wstandend(window)));
}
static JSVAL ncurses_standout(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(standout()));
}
static JSVAL ncurses_wstandout(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wstandout(window)));
}
static JSVAL ncurses_attr_get(JSARGS args) {
	HandleScope scope;
	Local<Object>ret = Object::New();
	attr_t attrs;
	short pair;
	ret->Set(String::New("success"), Integer::New(attr_get(&attrs, &pair, NULL)));
	ret->Set(String::New("attrs"), Integer::New(attrs));
	ret->Set(String::New("pair"), Integer::New(pair));
	return scope.Close(ret);
}
static JSVAL ncurses_wattr_get(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	Local<Object>ret = Object::New();
	attr_t attrs;
	short pair;
	ret->Set(String::New("success"), Integer::New(wattr_get(window, &attrs, &pair, NULL)));
	ret->Set(String::New("attrs"), Integer::New(attrs));
	ret->Set(String::New("pair"), Integer::New(pair));
	return scope.Close(ret);
}
static JSVAL ncurses_attr_off(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attr_off(args[0]->IntegerValue(), NULL)));
}
static JSVAL ncurses_wattr_off(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattr_off(window, args[1]->IntegerValue(), NULL)));
}
static JSVAL ncurses_attr_on(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attr_on(args[0]->IntegerValue(), NULL)));
}
static JSVAL ncurses_wattr_on(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattr_on(window, args[1]->IntegerValue(), NULL)));
}
static JSVAL ncurses_attr_set(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(attr_set(args[0]->IntegerValue(), args[1]->IntegerValue(), NULL)));
}
static JSVAL ncurses_wattr_set(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wattr_set(window, args[1]->IntegerValue(), args[2]->IntegerValue(), NULL)));
}
static JSVAL ncurses_chgat(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(chgat(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), NULL)));
}
static JSVAL ncurses_wchgat(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wchgat(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), NULL)));
}
static JSVAL ncurses_mvchgat(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(mvchgat(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), NULL)));
}
static JSVAL ncurses_mvwchgat(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwchgat(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), NULL)));
}
// macros
static JSVAL ncurses_COLOR_PAIR(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(COLOR_PAIR(args[0]->IntegerValue())));
}
static JSVAL ncurses_PAIR_NUMBER(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(PAIR_NUMBER(args[0]->IntegerValue())));
}
static JSVAL ncurses_KEY_F(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(KEY_F(args[0]->IntegerValue())));
}

// see man printw

/*
 * printw(s) - does not support printf() style format strings, varargs
 */
static JSVAL ncurses_printw(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[0]->ToString());
	return scope.Close(Integer::New(printw("%s", *str)));
}
/*
 * wprintw(window, s) - does not support printf() style format strings, varargs
 */
static JSVAL ncurses_wprintw(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[1]->ToString());
	return scope.Close(Integer::New(wprintw(window, "%s", *str)));
}
/*
 * mvprintw(y, x, s) - does not support printf() style format strings, varargs
 */
static JSVAL ncurses_mvprintw(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[2]->ToString());
	return scope.Close(Integer::New(mvprintw(args[0]->IntegerValue(), args[1]->IntegerValue(), "%s", *str)));
}
/*
 * mvwprintw(window, y, x, s) - does not support printf() style format strings, varargs
 */
static JSVAL ncurses_mvwprintw(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[3]->ToString());
	return scope.Close(Integer::New(mvwprintw(window, args[1]->IntegerValue(), args[2]->IntegerValue(), "%s", *str)));
}

// see man refresh

static JSVAL ncurses_refresh(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(refresh()));
}
static JSVAL ncurses_wrefresh(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wrefresh(window)));
}
static JSVAL ncurses_wnoutrefresh(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wnoutrefresh(window)));
}
static JSVAL ncurses_doupdate(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(doupdate()));
}
static JSVAL ncurses_redrawwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(redrawwin(window)));
}
static JSVAL ncurses_wredrawln(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wredrawln(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}

// see man addch
static JSVAL ncurses_addch(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(addch(args[0]->IntegerValue())));
}
static JSVAL ncurses_waddch(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(waddch(window, args[1]->IntegerValue())));
}
static JSVAL ncurses_mvaddch(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(mvaddch(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_mvwaddch(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwaddch(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue())));
}
static JSVAL ncurses_echochar(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(echochar(args[0]->IntegerValue())));
}
static JSVAL ncurses_wechochar(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wechochar(window, args[1]->IntegerValue())));
}

// see man move
static JSVAL ncurses_move(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(move(args[0]->IntegerValue(), args[1]->IntegerValue())));
}
static JSVAL ncurses_wmove(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wmove(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}

//.see man getyx
// these return { success: int, row: row, col: col, y: row, x: col }
// if window is not passed, then stdscr is used.
static JSVAL ncurses_getyx(JSARGS args) {
	HandleScope scope;
	WINDOW *window = stdscr;
	if (args.Length() > 0) {
		Local<External>wrap = Local<External>::Cast(args[0]);
		window = (WINDOW *)wrap->Value();
	}
	Local<Object>o = Object::New();
	int row, col;
	o->Set(String::New("success"), Integer::New(getyx(window, row, col)));
	o->Set(String::New("row"), Integer::New(row));
	o->Set(String::New("col"), Integer::New(col));
	o->Set(String::New("y"), Integer::New(row));
	o->Set(String::New("x"), Integer::New(col));
	return scope.Close(o);
}
static JSVAL ncurses_getparyx(JSARGS args) {
	HandleScope scope;
	WINDOW *window = stdscr;
	if (args.Length() > 0) {
		Local<External>wrap = Local<External>::Cast(args[0]);
		window = (WINDOW *)wrap->Value();
	}
	Local<Object>o = Object::New();
	int row, col;
	o->Set(String::New("success"), Integer::New(getparyx(window, row, col)));
	o->Set(String::New("row"), Integer::New(row));
	o->Set(String::New("col"), Integer::New(col));
	o->Set(String::New("y"), Integer::New(row));
	o->Set(String::New("x"), Integer::New(col));
	return scope.Close(o);
}
static JSVAL ncurses_getbegyx(JSARGS args) {
	HandleScope scope;
	WINDOW *window = stdscr;
	if (args.Length() > 0) {
		Local<External>wrap = Local<External>::Cast(args[0]);
		window = (WINDOW *)wrap->Value();
	}
	Local<Object>o = Object::New();
	int row, col;
	o->Set(String::New("success"), Integer::New(getbegyx(window, row, col)));
	o->Set(String::New("row"), Integer::New(row));
	o->Set(String::New("col"), Integer::New(col));
	o->Set(String::New("y"), Integer::New(row));
	o->Set(String::New("x"), Integer::New(col));
	return scope.Close(o);
}
static JSVAL ncurses_getmaxyx(JSARGS args) {
	HandleScope scope;
	WINDOW *window = stdscr;
	if (args.Length() > 0) {
		Local<External>wrap = Local<External>::Cast(args[0]);
		window = (WINDOW *)wrap->Value();
	}
	Local<Object>o = Object::New();
	int row, col;
	o->Set(String::New("success"), Integer::New(getmaxyx(window, row, col)));
	o->Set(String::New("row"), Integer::New(row));
	o->Set(String::New("col"), Integer::New(col));
	o->Set(String::New("y"), Integer::New(row));
	o->Set(String::New("x"), Integer::New(col));
	return scope.Close(o);
}

// see man addstr
static JSVAL ncurses_addstr(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[0]->ToString());
	return scope.Close(Integer::New(addstr(*str)));
}
static JSVAL ncurses_addnstr(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[0]->ToString());
	return scope.Close(Integer::New(addnstr(*str, args[1]->IntegerValue())));
}
static JSVAL ncurses_waddstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[1]->ToString());
	return scope.Close(Integer::New(waddstr(window, *str)));
}
static JSVAL ncurses_waddnstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[1]->ToString());
	return scope.Close(Integer::New(waddnstr(window, *str, args[2]->IntegerValue())));
}
static JSVAL ncurses_mvaddstr(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[2]->ToString());
	return scope.Close(Integer::New(mvaddstr(args[0]->IntegerValue(), args[1]->IntegerValue(), *str)));
}
static JSVAL ncurses_mvaddnstr(JSARGS args) {
	HandleScope scope;
    String::Utf8Value str(args[2]->ToString());
	return scope.Close(Integer::New(mvaddnstr(args[0]->IntegerValue(), args[1]->IntegerValue(), *str, args[3]->IntegerValue())));
}
static JSVAL ncurses_mvwaddstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[3]->ToString());
	return scope.Close(Integer::New(mvwaddstr(window, args[1]->IntegerValue(), args[2]->IntegerValue(), *str)));
}
static JSVAL ncurses_mvwaddnstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
    String::Utf8Value str(args[3]->ToString());
	return scope.Close(Integer::New(mvwaddnstr(window, args[1]->IntegerValue(), args[2]->IntegerValue(), *str, args[4]->IntegerValue())));
}

// see man getch
static JSVAL ncurses_getch(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(getch()));
}
static JSVAL ncurses_wgetch(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wgetch(window)));
}
static JSVAL ncurses_mvgetch(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(mvgetch(args[0]->IntegerValue(), args[1]->IntegerValue())));
}
static JSVAL ncurses_mvwgetch(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwgetch(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_ungetch(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(ungetch(args[0]->IntegerValue())));
}
static JSVAL ncurses_has_key(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(has_key(args[0]->IntegerValue())));
}

// see man getstr
static JSVAL ncurses_getstr(JSARGS args) {
	HandleScope scope;
	char buf[1024*1024];
	int ret = getstr(buf);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_getnstr(JSARGS args) {
	HandleScope scope;
	int n = args[0]->IntegerValue();
	char buf[n+1];
	int ret = getnstr(buf, n);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_wgetstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	char buf[1024*1024];
	int ret = wgetstr(window, buf);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_wgetnstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	int n = args[1]->IntegerValue();
	char buf[n+1];
	int ret = wgetnstr(window, buf, n);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_mvgetstr(JSARGS args) {
	HandleScope scope;
	char buf[1024*1024];
	int ret = mvgetstr(args[0]->IntegerValue(), args[1]->IntegerValue(), buf);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_mvgetnstr(JSARGS args) {
	HandleScope scope;
	int n = args[2]->IntegerValue();
	char buf[n+1];
	int ret = mvgetnstr(args[0]->IntegerValue(), args[1]->IntegerValue(), buf, n);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_mvwgetstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	char buf[1024*1024];
	int ret = mvwgetstr(window, args[1]->IntegerValue(), args[2]->IntegerValue(), buf);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}
static JSVAL ncurses_mvwgetnstr(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	int n = args[3]->IntegerValue();
	char buf[n+1];
	int ret = mvwgetnstr(window, args[1]->IntegerValue(), args[2]->IntegerValue(), buf, n);
	if (ret != OK) {
		return scope.Close(Integer::New(ERR));
	}
	return scope.Close(String::New(buf));
}

// see man erase
static JSVAL ncurses_erase(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(erase()));
}
static JSVAL ncurses_werase(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(werase(window)));
}
static JSVAL ncurses_clear(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(clear()));
}
static JSVAL ncurses_wclear(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wclear(window)));
}
static JSVAL ncurses_clrtobot(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(clrtobot()));
}
static JSVAL ncurses_wclrtobot(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wclrtobot(window)));
}
static JSVAL ncurses_clrtoeol(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(clrtoeol()));
}
static JSVAL ncurses_wclrtoeol(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wclrtoeol(window)));
}

// see man start_color
static JSVAL ncurses_start_color(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(start_color()));
}
static JSVAL ncurses_init_pair(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(init_pair(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_init_color(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(init_color(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue())));
}
static JSVAL ncurses_has_colors(JSARGS args) {
	HandleScope scope;
	return scope.Close(Boolean::New(has_colors()));
}
static JSVAL ncurses_can_change_color(JSARGS args) {
	HandleScope scope;
	return scope.Close(Boolean::New(can_change_color()));
}
static JSVAL ncurses_color_content(JSARGS args) {
	HandleScope scope;
	short r, g, b;
	int ret = color_content(args[0]->IntegerValue(), &r, &g, &b);
	Local<Object>o = Object::New();
	o->Set(String::New("success"), Integer::New(ret));
	o->Set(String::New("r"), Integer::New(r));
	o->Set(String::New("g"), Integer::New(g));
	o->Set(String::New("b"), Integer::New(b));
	return scope.Close(o);
}
static JSVAL ncurses_pair_content(JSARGS args) {
	HandleScope scope;
	short f, b;
	int ret = pair_content(args[0]->IntegerValue(), &f, &b);
	Local<Object>o = Object::New();
	o->Set(String::New("success"), Integer::New(ret));
	o->Set(String::New("f"), Integer::New(f));
	o->Set(String::New("b"), Integer::New(b));
	return scope.Close(o);
}

// see man newwin
static JSVAL ncurses_newwin(JSARGS args) {
	HandleScope scope;
	return scope.Close(External::New(newwin(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue())));
}
static JSVAL ncurses_delwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(delwin(window)));
}
static JSVAL ncurses_mvwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwin(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_subwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(External::New(subwin(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue())));
}
static JSVAL ncurses_derwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(External::New(derwin(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue())));
}
static JSVAL ncurses_mvderwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvderwin(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_dupwin(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(External::New(dupwin(window)));
}
static JSVAL ncurses_wsyncup(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	wsyncup(window);
	return Undefined();
}
static JSVAL ncurses_syncok(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(syncok(window, args[1]->BooleanValue())));
}
static JSVAL ncurses_wcursyncup(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	wcursyncup(window);
	return Undefined();
}
static JSVAL ncurses_wsyncdown(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	wsyncdown(window);
	return Undefined();
}

// see man border

static JSVAL ncurses_border(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(border(
		args[0]->IntegerValue(),	// ls
		args[1]->IntegerValue(),	// rs 
		args[2]->IntegerValue(),	// ts 
		args[3]->IntegerValue(),	// bs
		args[4]->IntegerValue(),	// tl
		args[5]->IntegerValue(),	// tr
		args[6]->IntegerValue(),	// bl
		args[7]->IntegerValue()		// br
	)));
}
static JSVAL ncurses_wborder(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wborder(
		window,
		JSCHAR(args[1]),	// ls
		JSCHAR(args[2]),	// ls
		JSCHAR(args[3]),	// ls
		JSCHAR(args[4]),	// ls
		JSCHAR(args[5]),	// ls
		JSCHAR(args[6]),	// ls
		JSCHAR(args[7]),	// ls
		JSCHAR(args[8])	// ls
//		args[2]->IntegerValue(),	// rs 
//		args[3]->IntegerValue(),	// ts 
//		args[4]->IntegerValue(),	// bs
//		args[5]->IntegerValue(),	// tl
//		args[6]->IntegerValue(),	// tr
//		args[7]->IntegerValue(),	// bl
//		args[8]->IntegerValue()		// br
	)));
}
static JSVAL ncurses_box(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(box(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_hline(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(hline(args[0]->IntegerValue(), args[1]->IntegerValue())));
}
static JSVAL ncurses_whline(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(whline(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_vline(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(vline(args[0]->IntegerValue(), args[1]->IntegerValue())));
}
static JSVAL ncurses_wvline(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(wvline(window, args[1]->IntegerValue(), args[2]->IntegerValue())));
}
static JSVAL ncurses_mvhline(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(mvhline(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue())));
}
static JSVAL ncurses_mvwhline(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwhline(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue())));
}
static JSVAL ncurses_mvvline(JSARGS args) {
	HandleScope scope;
	return scope.Close(Integer::New(mvvline(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue())));
}
static JSVAL ncurses_mvwvline(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	WINDOW *window = (WINDOW *)wrap->Value();
	return scope.Close(Integer::New(mvwvline(window, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue())));
}



// Accessors for curses global variables
static JSVAL  ncurses_get_COLOR_PAIRS(Local<String>property, const AccessorInfo& info) {
	return Integer::New(COLOR_PAIRS);
}
void ncurses_set_COLOR_PAIRS(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	COLOR_PAIRS = value->IntegerValue();
}
static JSVAL  ncurses_get_COLORS(Local<String>property, const AccessorInfo& info) {
	return Integer::New(COLORS);
}
void ncurses_set_COLORS(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	COLORS = value->IntegerValue();
}
static JSVAL  ncurses_get_COLS(Local<String>property, const AccessorInfo& info) {
	return Integer::New(COLS);
}
void ncurses_set_COLS(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	COLS = value->IntegerValue();
}
static JSVAL  ncurses_get_ESCDELAY(Local<String>property, const AccessorInfo& info) {
	return Integer::New(ESCDELAY);
}
void ncurses_set_ESCDELAY(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	ESCDELAY = value->IntegerValue();
}
static JSVAL  ncurses_get_LINES(Local<String>property, const AccessorInfo& info) {
	return Integer::New(LINES);
}
void ncurses_set_LINES(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	LINES = value->IntegerValue();
}
static JSVAL  ncurses_get_TABSIZE(Local<String>property, const AccessorInfo& info) {
	return Integer::New(TABSIZE);
}
void ncurses_set_TABSIZE(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	TABSIZE = value->IntegerValue();
}
static JSVAL  ncurses_get_curscr(Local<String>property, const AccessorInfo& info) {
	return External::New(curscr);
}
void ncurses_set_curscr(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	Local<External>wrap = Local<External>::Cast(value);
	curscr = (WINDOW *)wrap->Value();
}
static JSVAL  ncurses_get_newscr(Local<String>property, const AccessorInfo& info) {
	return External::New(newscr);
}
void ncurses_set_newscr(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	Local<External>wrap = Local<External>::Cast(value);
	newscr = (WINDOW *)wrap->Value();
}
static JSVAL  ncurses_get_stdscr(Local<String>property, const AccessorInfo& info) {
	return External::New(stdscr);
}
void ncurses_set_stdscr(Local<String>property, Local<Value>value, const AccessorInfo& info) {
	Local<External>wrap = Local<External>::Cast(value);
	stdscr = (WINDOW *)wrap->Value();
}

void init_ncurses_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>ncurses = ObjectTemplate::New();
	
	ncurses->Set(String::New("ERR"), Integer::New(ERR));
	ncurses->Set(String::New("OK"), Integer::New(OK));
	
	ncurses->Set(String::New("initscr"), FunctionTemplate::New(ncurses_initscr));
	ncurses->Set(String::New("endwin"), FunctionTemplate::New(ncurses_endwin));
	ncurses->Set(String::New("isendwin"), FunctionTemplate::New(ncurses_isendwin));
	ncurses->Set(String::New("newterm"), FunctionTemplate::New(ncurses_newterm));
	ncurses->Set(String::New("set_term"), FunctionTemplate::New(ncurses_set_term));
	ncurses->Set(String::New("delscreen"), FunctionTemplate::New(ncurses_delscreen));

	ncurses->Set(String::New("cbreak"), FunctionTemplate::New(ncurses_cbreak));
	ncurses->Set(String::New("nocbreak"), FunctionTemplate::New(ncurses_nocbreak));
	ncurses->Set(String::New("echo"), FunctionTemplate::New(ncurses_echo));
	ncurses->Set(String::New("noecho"), FunctionTemplate::New(ncurses_noecho));
	ncurses->Set(String::New("halfdelay"), FunctionTemplate::New(ncurses_halfdelay));
	ncurses->Set(String::New("intrflush"), FunctionTemplate::New(ncurses_intrflush));
	ncurses->Set(String::New("keypad"), FunctionTemplate::New(ncurses_keypad));
	ncurses->Set(String::New("meta"), FunctionTemplate::New(ncurses_meta));
	ncurses->Set(String::New("nodelay"), FunctionTemplate::New(ncurses_nodelay));
	ncurses->Set(String::New("raw"), FunctionTemplate::New(ncurses_raw));
	ncurses->Set(String::New("noraw"), FunctionTemplate::New(ncurses_noraw));
	ncurses->Set(String::New("noqiflush"), FunctionTemplate::New(ncurses_noqiflush));
	ncurses->Set(String::New("qiflush"), FunctionTemplate::New(ncurses_qiflush));
	ncurses->Set(String::New("notimeout"), FunctionTemplate::New(ncurses_notimeout));
	ncurses->Set(String::New("timeout"), FunctionTemplate::New(ncurses_timeout));
	ncurses->Set(String::New("wtimeout"), FunctionTemplate::New(ncurses_wtimeout));
	ncurses->Set(String::New("typeahead"), FunctionTemplate::New(ncurses_typeahead));

	ncurses->Set(String::New("attroff"), FunctionTemplate::New(ncurses_attroff));
	ncurses->Set(String::New("wattroff"), FunctionTemplate::New(ncurses_wattroff));
	ncurses->Set(String::New("attron"), FunctionTemplate::New(ncurses_attron));
	ncurses->Set(String::New("wattron"), FunctionTemplate::New(ncurses_wattron));
	ncurses->Set(String::New("attrset"), FunctionTemplate::New(ncurses_attrset));
	ncurses->Set(String::New("wattrset"), FunctionTemplate::New(ncurses_wattrset));
	ncurses->Set(String::New("color_set"), FunctionTemplate::New(ncurses_color_set));
	ncurses->Set(String::New("wcolor_set"), FunctionTemplate::New(ncurses_wcolor_set));
	ncurses->Set(String::New("standend"), FunctionTemplate::New(ncurses_standend));
	ncurses->Set(String::New("wstandend"), FunctionTemplate::New(ncurses_wstandend));
	ncurses->Set(String::New("standout"), FunctionTemplate::New(ncurses_standout));
	ncurses->Set(String::New("wstandout"), FunctionTemplate::New(ncurses_wstandout));
	ncurses->Set(String::New("attr_get"), FunctionTemplate::New(ncurses_attr_get));
	ncurses->Set(String::New("wattr_get"), FunctionTemplate::New(ncurses_wattr_get));
	ncurses->Set(String::New("attr_off"), FunctionTemplate::New(ncurses_attr_off));
	ncurses->Set(String::New("wattr_off"), FunctionTemplate::New(ncurses_wattr_off));
	ncurses->Set(String::New("attr_on"), FunctionTemplate::New(ncurses_attr_on));
	ncurses->Set(String::New("wattr_on"), FunctionTemplate::New(ncurses_wattr_on));
	ncurses->Set(String::New("attr_set"), FunctionTemplate::New(ncurses_attr_set));
	ncurses->Set(String::New("wattr_set"), FunctionTemplate::New(ncurses_wattr_set));
	ncurses->Set(String::New("chgat"), FunctionTemplate::New(ncurses_chgat));
	ncurses->Set(String::New("wchgat"), FunctionTemplate::New(ncurses_wchgat));
	ncurses->Set(String::New("mvchgat"), FunctionTemplate::New(ncurses_mvchgat));
	ncurses->Set(String::New("mvwchgat"), FunctionTemplate::New(ncurses_mvwchgat));
	ncurses->Set(String::New("COLOR_PAIR"), FunctionTemplate::New(ncurses_COLOR_PAIR));
	ncurses->Set(String::New("PAIR_NUMBER"), FunctionTemplate::New(ncurses_PAIR_NUMBER));
	ncurses->Set(String::New("KEY_F"), FunctionTemplate::New(ncurses_KEY_F));
	ncurses->Set(String::New("A_NORMAL"), Integer::New(A_NORMAL));
	ncurses->Set(String::New("A_STANDOUT"), Integer::New(A_STANDOUT));
	ncurses->Set(String::New("A_NORMAL"), Integer::New(A_UNDERLINE));
	ncurses->Set(String::New("A_UNDERLINE"), Integer::New(A_REVERSE));
	ncurses->Set(String::New("A_BLINK"), Integer::New(A_BLINK));
	ncurses->Set(String::New("A_DIM"), Integer::New(A_DIM));
	ncurses->Set(String::New("A_BOLD"), Integer::New(A_BOLD));
	ncurses->Set(String::New("A_PROTECT"), Integer::New(A_PROTECT));
	ncurses->Set(String::New("A_INVIS"), Integer::New(A_INVIS));
	ncurses->Set(String::New("A_ALTCHARSET"), Integer::New(A_ALTCHARSET));
	ncurses->Set(String::New("A_CHARTEXT"), Integer::New(A_CHARTEXT));
	
	
	ncurses->Set(String::New("printw"), FunctionTemplate::New(ncurses_printw));
	ncurses->Set(String::New("wprintw"), FunctionTemplate::New(ncurses_wprintw));
	ncurses->Set(String::New("mvprintw"), FunctionTemplate::New(ncurses_mvprintw));
	ncurses->Set(String::New("mvwprintw"), FunctionTemplate::New(ncurses_mvwprintw));
	
	ncurses->Set(String::New("refresh"), FunctionTemplate::New(ncurses_refresh));
	ncurses->Set(String::New("wrefresh"), FunctionTemplate::New(ncurses_wrefresh));
	ncurses->Set(String::New("wnoutrefresh"), FunctionTemplate::New(ncurses_wnoutrefresh));
	ncurses->Set(String::New("doupdate"), FunctionTemplate::New(ncurses_doupdate));
	ncurses->Set(String::New("redrawwin"), FunctionTemplate::New(ncurses_redrawwin));
	ncurses->Set(String::New("wredrawln"), FunctionTemplate::New(ncurses_wredrawln));
	
	ncurses->Set(String::New("addch"), FunctionTemplate::New(ncurses_addch));
	ncurses->Set(String::New("waddch"), FunctionTemplate::New(ncurses_waddch));
	ncurses->Set(String::New("mvaddch"), FunctionTemplate::New(ncurses_mvaddch));
	ncurses->Set(String::New("mvwaddch"), FunctionTemplate::New(ncurses_mvwaddch));
	ncurses->Set(String::New("echochar"), FunctionTemplate::New(ncurses_echochar));
	ncurses->Set(String::New("wechochar"), FunctionTemplate::New(ncurses_wechochar));
	
	ncurses->Set(String::New("move"), FunctionTemplate::New(ncurses_move));
	ncurses->Set(String::New("wmove"), FunctionTemplate::New(ncurses_wmove));
	
	
	ncurses->Set(String::New("getyx"), FunctionTemplate::New(ncurses_getyx));
	ncurses->Set(String::New("getparyx"), FunctionTemplate::New(ncurses_getparyx));
	ncurses->Set(String::New("getbegyx"), FunctionTemplate::New(ncurses_getbegyx));
	ncurses->Set(String::New("getmaxyx"), FunctionTemplate::New(ncurses_getmaxyx));
	
	ncurses->Set(String::New("addstr"), FunctionTemplate::New(ncurses_addstr));
	ncurses->Set(String::New("addnstr"), FunctionTemplate::New(ncurses_addnstr));
	ncurses->Set(String::New("waddstr"), FunctionTemplate::New(ncurses_waddstr));
	ncurses->Set(String::New("waddnstr"), FunctionTemplate::New(ncurses_waddnstr));
	ncurses->Set(String::New("mvaddstr"), FunctionTemplate::New(ncurses_mvaddstr));
	ncurses->Set(String::New("mvaddnstr"), FunctionTemplate::New(ncurses_mvaddnstr));
	ncurses->Set(String::New("mvwaddstr"), FunctionTemplate::New(ncurses_mvwaddstr));
	ncurses->Set(String::New("mvwaddnstr"), FunctionTemplate::New(ncurses_mvwaddnstr));
	
	ncurses->Set(String::New("getch"), FunctionTemplate::New(ncurses_getch));
	ncurses->Set(String::New("wgetch"), FunctionTemplate::New(ncurses_wgetch));
	ncurses->Set(String::New("mvgetch"), FunctionTemplate::New(ncurses_mvgetch));
	ncurses->Set(String::New("mvwgetch"), FunctionTemplate::New(ncurses_mvwgetch));
	ncurses->Set(String::New("ungetch"), FunctionTemplate::New(ncurses_ungetch));
	ncurses->Set(String::New("has_key"), FunctionTemplate::New(ncurses_has_key));
	
	ncurses->Set(String::New("getstr"), FunctionTemplate::New(ncurses_getstr));
	ncurses->Set(String::New("getnstr"), FunctionTemplate::New(ncurses_getnstr));
	ncurses->Set(String::New("wgetstr"), FunctionTemplate::New(ncurses_wgetstr));
	ncurses->Set(String::New("wgetnstr"), FunctionTemplate::New(ncurses_wgetnstr));
	ncurses->Set(String::New("mvgetstr"), FunctionTemplate::New(ncurses_mvgetstr));
	ncurses->Set(String::New("mvgetnstr"), FunctionTemplate::New(ncurses_mvgetnstr));
	ncurses->Set(String::New("mvwgetstr"), FunctionTemplate::New(ncurses_mvwgetstr));
	ncurses->Set(String::New("mvwgetnstr"), FunctionTemplate::New(ncurses_mvwgetnstr));
	
	ncurses->Set(String::New("erase"), FunctionTemplate::New(ncurses_erase));
	ncurses->Set(String::New("werase"), FunctionTemplate::New(ncurses_werase));
	ncurses->Set(String::New("clear"), FunctionTemplate::New(ncurses_clear));
	ncurses->Set(String::New("wclear"), FunctionTemplate::New(ncurses_wclear));
	ncurses->Set(String::New("clrtobot"), FunctionTemplate::New(ncurses_clrtobot));
	ncurses->Set(String::New("wclrtobot"), FunctionTemplate::New(ncurses_wclrtobot));
	ncurses->Set(String::New("clrtoeol"), FunctionTemplate::New(ncurses_clrtoeol));
	ncurses->Set(String::New("wclrtoeol"), FunctionTemplate::New(ncurses_wclrtoeol));
	
	ncurses->Set(String::New("start_color"), FunctionTemplate::New(ncurses_start_color));
	ncurses->Set(String::New("init_pair"), FunctionTemplate::New(ncurses_init_pair));
	ncurses->Set(String::New("init_color"), FunctionTemplate::New(ncurses_init_color));
	ncurses->Set(String::New("has_colors"), FunctionTemplate::New(ncurses_has_colors));
	ncurses->Set(String::New("can_change_color"), FunctionTemplate::New(ncurses_can_change_color));
	ncurses->Set(String::New("color_content"), FunctionTemplate::New(ncurses_color_content));
	ncurses->Set(String::New("pair_content"), FunctionTemplate::New(ncurses_pair_content));
	
	ncurses->Set(String::New("newwin"), FunctionTemplate::New(ncurses_newwin));
	ncurses->Set(String::New("delwin"), FunctionTemplate::New(ncurses_delwin));
	ncurses->Set(String::New("mvwin"), FunctionTemplate::New(ncurses_mvwin));
	ncurses->Set(String::New("subwin"), FunctionTemplate::New(ncurses_subwin));
	ncurses->Set(String::New("derwin"), FunctionTemplate::New(ncurses_derwin));
	ncurses->Set(String::New("mvderwin"), FunctionTemplate::New(ncurses_mvderwin));
	ncurses->Set(String::New("dupwin"), FunctionTemplate::New(ncurses_dupwin));
	ncurses->Set(String::New("wsyncup"), FunctionTemplate::New(ncurses_wsyncup));
	ncurses->Set(String::New("syncok"), FunctionTemplate::New(ncurses_syncok));
	ncurses->Set(String::New("wcursyncup"), FunctionTemplate::New(ncurses_wcursyncup));
	ncurses->Set(String::New("wsyncdown"), FunctionTemplate::New(ncurses_wsyncdown));
	
	ncurses->Set(String::New("border"), FunctionTemplate::New(ncurses_border));
	ncurses->Set(String::New("wborder"), FunctionTemplate::New(ncurses_wborder));
	ncurses->Set(String::New("box"), FunctionTemplate::New(ncurses_box));
	ncurses->Set(String::New("hline"), FunctionTemplate::New(ncurses_hline));
	ncurses->Set(String::New("whline"), FunctionTemplate::New(ncurses_whline));
	ncurses->Set(String::New("vline"), FunctionTemplate::New(ncurses_vline));
	ncurses->Set(String::New("wvline"), FunctionTemplate::New(ncurses_wvline));
	ncurses->Set(String::New("mvhline"), FunctionTemplate::New(ncurses_mvhline));
	ncurses->Set(String::New("mvwhline"), FunctionTemplate::New(ncurses_mvwhline));
	ncurses->Set(String::New("mvvline"), FunctionTemplate::New(ncurses_mvvline));
	ncurses->Set(String::New("mvwvline"), FunctionTemplate::New(ncurses_mvwvline));
	
	
	// KEY CODES
	ncurses->Set(String::New("KEY_CODE_YES"), Integer::New(KEY_CODE_YES));	/* A wchar_t contains a key code */
	ncurses->Set(String::New("KEY_MIN"), Integer::New(KEY_MIN));		/* Minimum curses key */
	ncurses->Set(String::New("KEY_BREAK"), Integer::New(KEY_BREAK));		/* Break key (unreliable) */
	ncurses->Set(String::New("KEY_SRESET"), Integer::New(KEY_SRESET));	/* Soft (partial) reset (unreliable) */
	ncurses->Set(String::New("KEY_RESET"), Integer::New(KEY_RESET));	/* Reset or hard reset (unreliable) */
	ncurses->Set(String::New("KEY_DOWN"), Integer::New(KEY_DOWN));	/* down-arrow key */
	ncurses->Set(String::New("KEY_UP"), Integer::New(KEY_UP));		/* up-arrow key */
	ncurses->Set(String::New("KEY_LEFT"), Integer::New(KEY_LEFT));	/* left-arrow key */
	ncurses->Set(String::New("KEY_RIGHT"), Integer::New(KEY_RIGHT));	/* right-arrow key */
	ncurses->Set(String::New("KEY_HOME"), Integer::New(KEY_HOME));	/* home key */
	ncurses->Set(String::New("KEY_BACKSPACE"), Integer::New(KEY_BACKSPACE));	/* backspace key */
	ncurses->Set(String::New("KEY_F0"), Integer::New(KEY_F0));		/* Function keys.  Space for 64 */
	ncurses->Set(String::New("KEY_DL"), Integer::New(KEY_DL));		/* delete-line key */
	ncurses->Set(String::New("KEY_IL"), Integer::New(KEY_IL));		/* insert-line key */
	ncurses->Set(String::New("KEY_DC"), Integer::New(KEY_DC));		/* delete-character key */
	ncurses->Set(String::New("KEY_IC"), Integer::New(KEY_IC));		/* insert-character key */
	ncurses->Set(String::New("KEY_EIC"), Integer::New(KEY_EIC));		/* sent by rmir or smir in insert mode */
	ncurses->Set(String::New("KEY_CLEAR"), Integer::New(KEY_CLEAR));	/* clear-screen or erase key */
	ncurses->Set(String::New("KEY_EOS"), Integer::New(KEY_EOS));		/* clear-to-end-of-screen key */
	ncurses->Set(String::New("KEY_EOL"), Integer::New(KEY_EOL));		/* clear-to-end-of-line key */
	ncurses->Set(String::New("KEY_SF"), Integer::New(KEY_SF));		/* scroll-forward key */
	ncurses->Set(String::New("KEY_SR"), Integer::New(KEY_SR));		/* scroll-backward key */
	ncurses->Set(String::New("KEY_NPAGE"), Integer::New(KEY_NPAGE));	/* next-page key */
	ncurses->Set(String::New("KEY_PPAGE"), Integer::New(KEY_PPAGE));	/* previous-page key */
	ncurses->Set(String::New("KEY_STAB"), Integer::New(KEY_STAB));	/* set-tab key */
	ncurses->Set(String::New("KEY_CTAB"), Integer::New(KEY_CTAB));	/* clear-tab key */
	ncurses->Set(String::New("KEY_CATAB"), Integer::New(KEY_CATAB));	/* clear-all-tabs key */
	ncurses->Set(String::New("KEY_ENTER"), Integer::New(KEY_ENTER));	/* enter/send key */
	ncurses->Set(String::New("KEY_PRINT"), Integer::New(KEY_PRINT));	/* print key */
	ncurses->Set(String::New("KEY_LL"), Integer::New(KEY_LL));		/* lower-left key (home down) */
	ncurses->Set(String::New("KEY_A1"), Integer::New(KEY_A1));		/* upper left of keypad */
	ncurses->Set(String::New("KEY_A3"), Integer::New(KEY_A3));		/* upper right of keypad */
	ncurses->Set(String::New("KEY_B2"), Integer::New(KEY_B2));		/* center of keypad */
	ncurses->Set(String::New("KEY_C1"), Integer::New(KEY_C1));		/* lower left of keypad */
	ncurses->Set(String::New("KEY_C3"), Integer::New(KEY_C3));		/* lower right of keypad */
	ncurses->Set(String::New("KEY_BTAB"), Integer::New(KEY_BTAB));	/* back-tab key */
	ncurses->Set(String::New("KEY_BEG"), Integer::New(KEY_BEG));		/* begin key */
	ncurses->Set(String::New("KEY_CANCEL"), Integer::New(KEY_CANCEL));	/* cancel key */
	ncurses->Set(String::New("KEY_CLOSE"), Integer::New(KEY_CLOSE));	/* close key */
	ncurses->Set(String::New("KEY_COMMAND"), Integer::New(KEY_COMMAND));	/* command key */
	ncurses->Set(String::New("KEY_COPY"), Integer::New(KEY_COPY));	/* copy key */
	ncurses->Set(String::New("KEY_CREATE"), Integer::New(KEY_CREATE));	/* create key */
	ncurses->Set(String::New("KEY_END"), Integer::New(KEY_END));		/* end key */
	ncurses->Set(String::New("KEY_EXIT"), Integer::New(KEY_EXIT));	/* exit key */
	ncurses->Set(String::New("KEY_FIND"), Integer::New(KEY_FIND));	/* find key */
	ncurses->Set(String::New("KEY_HELP"), Integer::New(KEY_HELP));	/* help key */
	ncurses->Set(String::New("KEY_MARK"), Integer::New(KEY_MARK));	/* mark key */
	ncurses->Set(String::New("KEY_MESSAGE"), Integer::New(KEY_MESSAGE));	/* message key */
	ncurses->Set(String::New("KEY_MOVE"), Integer::New(KEY_MOVE));	/* move key */
	ncurses->Set(String::New("KEY_NEXT"), Integer::New(KEY_NEXT));	/* next key */
	ncurses->Set(String::New("KEY_OPEN"), Integer::New(KEY_OPEN));	/* open key */
	ncurses->Set(String::New("KEY_OPTIONS"), Integer::New(KEY_OPTIONS));	/* options key */
	ncurses->Set(String::New("KEY_PREVIOUS"), Integer::New(KEY_PREVIOUS));	/* previous key */
	ncurses->Set(String::New("KEY_REDO"), Integer::New(KEY_REDO));	/* redo key */
	ncurses->Set(String::New("KEY_REFERENCE"), Integer::New(KEY_REFERENCE));	/* reference key */
	ncurses->Set(String::New("KEY_REFRESH"), Integer::New(KEY_REFRESH));	/* refresh key */
	ncurses->Set(String::New("KEY_REPLACE"), Integer::New(KEY_REPLACE));	/* replace key */
	ncurses->Set(String::New("KEY_RESTART"), Integer::New(KEY_RESTART));	/* restart key */
	ncurses->Set(String::New("KEY_RESUME"), Integer::New(KEY_RESUME));	/* resume key */
	ncurses->Set(String::New("KEY_SAVE"), Integer::New(KEY_SAVE));	/* save key */
	ncurses->Set(String::New("KEY_SBEG"), Integer::New(KEY_SBEG));	/* shifted begin key */
	ncurses->Set(String::New("KEY_SCANCEL"), Integer::New(KEY_SCANCEL));	/* shifted cancel key */
	ncurses->Set(String::New("KEY_SCOMMAND"), Integer::New(KEY_SCOMMAND));	/* shifted command key */
	ncurses->Set(String::New("KEY_SCOPY"), Integer::New(KEY_SCOPY));	/* shifted copy key */
	ncurses->Set(String::New("KEY_SCREATE"), Integer::New(KEY_SCREATE));	/* shifted create key */
	ncurses->Set(String::New("KEY_SDC"), Integer::New(KEY_SDC));		/* shifted delete-character key */
	ncurses->Set(String::New("KEY_SDL"), Integer::New(KEY_SDL));		/* shifted delete-line key */
	ncurses->Set(String::New("KEY_SELECT"), Integer::New(KEY_SELECT));	/* select key */
	ncurses->Set(String::New("KEY_SEND"), Integer::New(KEY_SEND));	/* shifted end key */
	ncurses->Set(String::New("KEY_SEOL"), Integer::New(KEY_SEOL));	/* shifted clear-to-end-of-line key */
	ncurses->Set(String::New("KEY_SEXIT"), Integer::New(KEY_SEXIT));	/* shifted exit key */
	ncurses->Set(String::New("KEY_SFIND"), Integer::New(KEY_SFIND));	/* shifted find key */
	ncurses->Set(String::New("KEY_SHELP"), Integer::New(KEY_SHELP));	/* shifted help key */
	ncurses->Set(String::New("KEY_SHOME"), Integer::New(KEY_SHOME));	/* shifted home key */
	ncurses->Set(String::New("KEY_SIC"), Integer::New(KEY_SIC));		/* shifted insert-character key */
	ncurses->Set(String::New("KEY_SLEFT"), Integer::New(KEY_SLEFT));	/* shifted left-arrow key */
	ncurses->Set(String::New("KEY_SMESSAGE"), Integer::New(KEY_SMESSAGE));	/* shifted message key */
	ncurses->Set(String::New("KEY_SMOVE"), Integer::New(KEY_SMOVE));	/* shifted move key */
	ncurses->Set(String::New("KEY_SNEXT"), Integer::New(KEY_SNEXT));	/* shifted next key */
	ncurses->Set(String::New("KEY_SOPTIONS"), Integer::New(KEY_SOPTIONS));	/* shifted options key */
	ncurses->Set(String::New("KEY_SPREVIOUS"), Integer::New(KEY_SPREVIOUS));	/* shifted previous key */
	ncurses->Set(String::New("KEY_SPRINT"), Integer::New(KEY_SPRINT));	/* shifted print key */
	ncurses->Set(String::New("KEY_SREDO"), Integer::New(KEY_SREDO));	/* shifted redo key */
	ncurses->Set(String::New("KEY_SREPLACE"), Integer::New(KEY_SREPLACE));	/* shifted replace key */
	ncurses->Set(String::New("KEY_SRIGHT"), Integer::New(KEY_SRIGHT));	/* shifted right-arrow key */
	ncurses->Set(String::New("KEY_SRSUME"), Integer::New(KEY_SRSUME));	/* shifted resume key */
	ncurses->Set(String::New("KEY_SSAVE"), Integer::New(KEY_SSAVE));	/* shifted save key */
	ncurses->Set(String::New("KEY_SSUSPEND"), Integer::New(KEY_SSUSPEND));	/* shifted suspend key */
	ncurses->Set(String::New("KEY_SUNDO"), Integer::New(KEY_SUNDO));	/* shifted undo key */
	ncurses->Set(String::New("KEY_SUSPEND"), Integer::New(KEY_SUSPEND));	/* suspend key */
	ncurses->Set(String::New("KEY_UNDO"), Integer::New(KEY_UNDO));	/* undo key */
	ncurses->Set(String::New("KEY_MOUSE"), Integer::New(KEY_MOUSE));	/* Mouse event has occurred */
	ncurses->Set(String::New("KEY_RESIZE"), Integer::New(KEY_RESIZE));	/* Terminal resize event */
	ncurses->Set(String::New("KEY_EVENT"), Integer::New(KEY_EVENT));	/* We were interrupted by an event */
	ncurses->Set(String::New("KEY_MAX"), Integer::New(KEY_MAX));		/* Maximum key value is 0633 */
	
	// Line Graphics
	ncurses->Set(String::New("ACS_ULCORNER"), Integer::New(ACS_ULCORNER));	/* upper left corner */
	ncurses->Set(String::New("ACS_LLCORNER"), Integer::New(ACS_LLCORNER));	/* lower left corner */
	ncurses->Set(String::New("ACS_URCORNER"), Integer::New(ACS_URCORNER));	/* upper right corner */
	ncurses->Set(String::New("ACS_LRCORNER"), Integer::New(ACS_LRCORNER));	/* lower right corner */
	ncurses->Set(String::New("ACS_LTEE"), Integer::New(ACS_LTEE));	/* tee pointing right */
	ncurses->Set(String::New("ACS_RTEE"), Integer::New(ACS_RTEE));	/* tee pointing left */
	ncurses->Set(String::New("ACS_BTEE"), Integer::New(ACS_BTEE));	/* tee pointing up */
	ncurses->Set(String::New("ACS_TTEE"), Integer::New(ACS_TTEE));	/* tee pointing down */
	ncurses->Set(String::New("ACS_HLINE"), Integer::New(ACS_HLINE));	/* horizontal line */
	ncurses->Set(String::New("ACS_VLINE"), Integer::New(ACS_VLINE));	/* vertical line */
	ncurses->Set(String::New("ACS_PLUS"), Integer::New(ACS_PLUS));	/* large plus or crossover */
	ncurses->Set(String::New("ACS_S1"), Integer::New(ACS_S1));	/* scan line 1 */
	ncurses->Set(String::New("ACS_S9"), Integer::New(ACS_S9));	/* scan line 9 */
	ncurses->Set(String::New("ACS_DIAMOND"), Integer::New(ACS_DIAMOND));	/* diamond */
	ncurses->Set(String::New("ACS_CKBOARD"), Integer::New(ACS_CKBOARD));	/* checker board (stipple) */
	ncurses->Set(String::New("ACS_DEGREE"), Integer::New(ACS_DEGREE));	/* degree symbol */
	ncurses->Set(String::New("ACS_PLMINUS"), Integer::New(ACS_PLMINUS));	/* plus/minus */
	ncurses->Set(String::New("ACS_BULLET"), Integer::New(ACS_BULLET));	/* bullet */
	/* Teletype 5410v1 symbols begin here */
	ncurses->Set(String::New("ACS_LARROW"), Integer::New(ACS_LARROW));	/* arrow pointing left */
	ncurses->Set(String::New("ACS_RARROW"), Integer::New(ACS_RARROW));	/* arrow pointing right */
	ncurses->Set(String::New("ACS_DARROW"), Integer::New(ACS_DARROW));	/* arrow pointing down */
	ncurses->Set(String::New("ACS_UARROW"), Integer::New(ACS_UARROW));	/* arrow pointing up */
	ncurses->Set(String::New("ACS_BOARD"), Integer::New(ACS_BOARD));	/* board of squares */
	ncurses->Set(String::New("ACS_LANTERN"), Integer::New(ACS_LANTERN));	/* lantern symbol */
	ncurses->Set(String::New("ACS_BLOCK"), Integer::New(ACS_BLOCK));	/* solid square block */
	/*
	 * These aren't documented, but a lot of System Vs have them anyway
	 * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
	 * The ACS_names may not match AT&T's, our source didn't know them.
	 */
	ncurses->Set(String::New("ACS_S3"), Integer::New(ACS_S3));	/* scan line 3 */
	ncurses->Set(String::New("ACS_S7"), Integer::New(ACS_S7));	/* scan line 7 */
	ncurses->Set(String::New("ACS_LEQUAL"), Integer::New(ACS_LEQUAL));	/* less/equal */
	ncurses->Set(String::New("ACS_GEQUAL"), Integer::New(ACS_GEQUAL));	/* greater/equal */
	ncurses->Set(String::New("ACS_PI"), Integer::New(ACS_PI));	/* Pi */
	ncurses->Set(String::New("ACS_NEQUAL"), Integer::New(ACS_NEQUAL));	/* not equal */
	ncurses->Set(String::New("ACS_STERLING"), Integer::New(ACS_STERLING));	/* UK pound sign */

	/*
	 * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
	 * is the right, b is the bottom, and l is the left.  t, r, b, and l might
	 * be B (blank), S (single), D (double), or T (thick).  The subset defined
	 * here only uses B and S.
	 */
	ncurses->Set(String::New("ACS_BSSB"), Integer::New(ACS_BSSB));
	ncurses->Set(String::New("ACS_SSBB"), Integer::New(ACS_SSBB));
	ncurses->Set(String::New("ACS_BBSS"), Integer::New(ACS_BBSS));
	ncurses->Set(String::New("ACS_SBBS"), Integer::New(ACS_SBBS));
	ncurses->Set(String::New("ACS_SBSS"), Integer::New(ACS_SBSS));
	ncurses->Set(String::New("ACS_SSSB"), Integer::New(ACS_SSSB));
	ncurses->Set(String::New("ACS_SSBS"), Integer::New(ACS_SSBS));
	ncurses->Set(String::New("ACS_BSSS"), Integer::New(ACS_BSSS));
	ncurses->Set(String::New("ACS_BSBS"), Integer::New(ACS_BSBS));
	ncurses->Set(String::New("ACS_SBSB"), Integer::New(ACS_SBSB));
	ncurses->Set(String::New("ACS_SSSS"), Integer::New(ACS_SSSS));
	
	
	// Accessors
	ncurses->SetAccessor(String::New("COLOR_PAIRS"), ncurses_get_COLOR_PAIRS, ncurses_set_COLOR_PAIRS);
	ncurses->SetAccessor(String::New("COLORS"), ncurses_get_COLORS, ncurses_set_COLORS);
	ncurses->SetAccessor(String::New("COLS"), ncurses_get_COLS, ncurses_set_COLS);
	ncurses->SetAccessor(String::New("ESCDELAY"), ncurses_get_ESCDELAY, ncurses_set_ESCDELAY);
	ncurses->SetAccessor(String::New("LINES"), ncurses_get_LINES, ncurses_set_LINES);
	ncurses->SetAccessor(String::New("TABSIZE"), ncurses_get_TABSIZE, ncurses_set_TABSIZE);
	ncurses->SetAccessor(String::New("curscr"), ncurses_get_curscr, ncurses_set_curscr);
	ncurses->SetAccessor(String::New("newscr"), ncurses_get_newscr, ncurses_set_newscr);
	ncurses->SetAccessor(String::New("stdscr"), ncurses_get_stdscr, ncurses_set_stdscr);
	
	globalObject->Set(String::New("ncurses"), ncurses);
}
