ARCH := $(shell getconf LONG_BIT)

RELEASE := $(shell lsb_release -sr)

GROUP = sudo

CORE=	main.o base64.o global.o console.o process.o net.o fs.o buffer.o v8.o http.o md5.o popen.o linenoise.o async.o time.o

OBJ=	mysql.o memcached.o gd.o ncurses.o sem.o logfile.o sqlite3.o xhrhelper.o curl.o ssh2.o sftp.o ftp.o ftplib.o editline.o cairo.o expat.o
#OBJ=	memcached.o gd.o ncurses.o sem.o logfile.o sqlite3.o xhrhelper.o curl.o ssh2.o sftp.o ftp.o ftplib.o editline.o cairo.o expat.o

V8DIR=	./v8-read-only

# V8LIB_64 := $(V8DIR)/out/x64.release/obj.target/tools/gyp
# V8LIB_32 := $(V8DIR)/out/ia32.release/obj.target/tools/gyp
V8LIB_64 := $(V8DIR)/out/x64.release/obj.target/tools/gyp
V8LIB_32 := $(V8DIR)/out/ia32.release/obj.target/tools/gyp
V8LIB_DIR := $(V8LIB_$(ARCH))

V8VERSION_64 := x64.release
V8VERSION_32 := ia32.release
V8VERSION := $(V8VERSION_$(ARCH))

V8=	$(V8LIB_DIR)/libv8_base.a $(V8LIB_DIR)/libv8_snapshot.a $(V8LIB_DIR)/libv8.so
# V8LIBS = -L$(V8LIB_DIR) -lv8_base -lv8_snapshot
V8LIBS = -L$(V8LIB_DIR) -lv8

INCDIRS=    -I$(V8DIR)/include -I/usr/include/mysql

LIBDIRS=    -L$(V8LIB_DIR)/ 

CFLAGS = -fexceptions -fomit-frame-pointer -fdata-sections -ffunction-sections -fno-strict-aliasing -fvisibility=hidden -Wall -W -Wno-ignored-qualifiers -Wno-unused-function -Wno-unused-parameter -Wnon-virtual-dtor -m$(ARCH) -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -ansi -fno-strict-aliasing -DNEW_MEMCACHED

%.o: %.cpp SilkJS.h 
	g++ $(CFLAGS) -c $(INCDIRS) -o $*.o $*.cpp

silkjs: deps $(V8DIR) $(V8) $(CORE) $(OBJ) SilkJS.h Makefile
	g++ -o silkjs $(CORE) $(OBJ) $(V8LIBS) -lmysqlclient -lmm -lgd -lncurses -lssl -lpthread -lsqlite3 -lcurl -lssh2 -lmemcached -lcairo -lexpat -ldl -Wl,-rpath=/usr/local/silkjs/src/v8,-rpath=$(V8LIB_DIR) 

deps: 
	sudo apt-get -y install libmm-dev libmysqlclient-dev libmemcached-dev libgd2-xpm-dev libncurses5-dev libsqlite3-dev libcurl4-openssl-dev libssh2-1-dev libcairo2-dev

debug:	    CFLAGS += -g
debug:	    silkjs
	
bootstrap:  CFLAGS += -DBOOTSTRAP_SILKJS

bootstrap:  $(V8) $(CORE) SilkJS.h Makefile
	g++ $(CFLAGS) -o bootstrap-silkjs $(CORE) -L$(V8LIB_DIR)/ -lv8_base -lv8_snapshot -lpthread

perms:
	sudo chgrp $(GROUP)  /usr/local /usr/local/bin 
	sudo chmod g+rwx /usr/local /usr/local/bin

install:	silkjs perms
	@rm -rf /usr/local/silkjs/builtin /usr/local/silkjs/bin /usr/local/silkjs/httpd /usr/local/silkjs/lib /usr/local/silkjs/modules /usr/local/silkjs/walkthrough /usr/local/silkjs/src
	@mkdir -p /usr/local/bin /usr/local/silkjs /usr/local/silkjs/src/v8 /usr/local/silkjs/projects /usr/local/silkjs/contrib
	cp -rp ../builtin ../examples ../httpd ../lib ../modules ../walkthrough /usr/local/silkjs
	cp ../src/*.cpp ../src/*.h /usr/local/silkjs/src
	cp -rp $(V8DIR)/include $(V8) /usr/local/silkjs/src/v8
	mv /usr/local/silkjs/examples /usr/local/silkjs/bin
	chmod 755 /usr/local/silkjs/bin/* /usr/local/silkjs/httpd/main.js
	cp silkjs /usr/local/silkjs/bin
	ln -sf /usr/local/silkjs/bin/silkjs /usr/local/bin
	ln -sf /usr/local/silkjs/httpd/main.js /usr/local/bin/httpd-silk.js
	
$(V8DIR):
	echo "svn checkout"
#	svn checkout http://v8.googlecode.com/svn/branches/bleeding_edge/ v8-read-only
	svn checkout http://v8.googlecode.com/svn/trunk/ v8-read-only

$(V8):
	# cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make GYPFLAGS="-Dcomponent=shared_library -Dv8_enable_extra_checks=0" -j 4 $(V8VERSION)
	cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make GYPFLAGS="-Dcomponent=shared_library" -j 4 $(V8VERSION)

update:
	cd $(V8DIR) && svn update && make dependencies && GYP_GENERATORS=make make $(V8VERSION)
	git pull
	
clean:
	rm -rf silkjs *.o

realclean:
	rm -rf silkjs *.o v8-read-only

release: silkjs
	tar czvfp ~/SilkJS.tgz silkjs examples httpd lib

$(OBJ): SilkJS.h Makefile

$(CORE): SilkJS.h Makefile
