CORE=	main.o base64.o global.o console.o process.o net.o fs.o buffer.o v8.o http.o md5.o 

OBJ=	mysql.o gd.o ncurses.o sem.o logfile.o sqlite3.o

V8DIR=	../v8-read-only

V8=	$(V8DIR)/out/x64.release/obj.target/tools/gyp/libv8_base.a $(V8DIR)/out/x64.release/obj.target/tools/gyp/libv8_snapshot.a

CFLAGS = -O6 -fomit-frame-pointer -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-rtti -fno-exceptions -fvisibility=hidden -Wall -W -Wno-unused-parameter -Wnon-virtual-dtor -m64 -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -ansi -fno-strict-aliasing

%.o: %.cpp SilkJS.h Makefile
	g++ $(CFLAGS) -c -I$(V8DIR)/include -I/usr/include/mysql -g -o $*.o $*.cpp

silkjs:	dependencies $(CORE) $(OBJ) SilkJS.h Makefile
	g++ $(CFLAGS) -o silkjs $(CORE) $(OBJ) -L$(V8DIR)/out/x64.release/obj.target/tools/gyp/ -lv8_base -lv8_snapshot -L/usr/local/lib/mysql -lmysqlclient -lmm -lgd -lncurses -lssl -lpthread -lsqlite3

bootstrap:  CFLAGS += -DBOOTSTRAP_SILKJS

bootstrap:  dependencies $(CORE) SilkJS.h Makefile
	g++ $(CFLAGS) -o bootstrap-silkjs $(CORE) -L$(V8DIR)/out/x64.release/obj.target/tools/gyp/ -lv8_base -lv8_snapshot -lpthread
	
dependencies:
	cd $(V8DIR) && make dependencies && GYP_GENERATORS=make make x64.release

update:
	cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make x64.release
	git pull
	
clean:
	rm -f silkjs *.o

release: silkjs
	tar czvfp ~/SilkJS.tgz silkjs examples httpd lib

$(OBJ): SilkJS.h Makefile

$(CORE): SilkJS.h Makefile
