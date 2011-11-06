// see http://invisible-island.net/ncurses/man/ncurses.3x.html

#include "SilkJS.h"
#include <ncurses.h>

void init_ncurses_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>ncurses = ObjectTemplate::New();
	
	globalObject->Set(String::New("ncurses"), ncurses);
}
