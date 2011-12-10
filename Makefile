OBJ=	main.o base64.o global.o console.o process.o net.o fs.o buffer.o mysql.o http.o gd.o ncurses.o sem.o logfile.o v8.o md5.o

CFLAGS = -O6 -fomit-frame-pointer -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-rtti -fno-exceptions -fvisibility=hidden -Wall -W -Wno-unused-parameter -Wnon-virtual-dtor -m64 -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -ansi -fno-strict-aliasing

%.o: %.cpp SilkJS.h Makefile
	g++ $(CFLAGS) -c -I../v8-read-only/include -I/usr/include/mysql -g -o $*.o $*.cpp

silkjs:	$(OBJ) SilkJS.h Makefile
	g++ $(CFLAGS) -o silkjs $(OBJ) -L../v8-read-only/out/x64.release/obj.target/tools/gyp/ -lv8_base -lv8_snapshot -L/usr/local/lib/mysql -lmysqlclient -lmm -lgd -lncurses -lssl -lpthread

clean:
	rm -f silkjs *.o

release: silkjs
	tar czvfp ~/SilkJS.tgz silkjs examples httpd lib

$(OBJ): SilkJS.h Makefile
