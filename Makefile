UNAME := $(shell uname -s)
MAKEFILE=Makefile
ifeq ($(UNAME),Darwin)
	MAKEFILE=Makefile.osx
else
    ifneq ($(shell shtool platform | grep -i "suse"),)
		MAKEFILE=Makefile.sles
    endif
    ifneq ($(shell shtool platform | grep -i "centos"),)
		MAKEFILE=Makefile.centos
    endif
    ifneq ($(shell shtool platform | grep -i "linuxmint"),)
		MAKEFILE=Makefile.Mint
    endif
    ifneq ($(shell shtool platform | grep -i "ubuntu"),)
		MAKEFILE=Makefile
    endif
endif

all:
	cd src && make -f$(MAKEFILE)
	cp src/silkjs .

bootstrap:
	cd src && make -f$(MAKEFILE) bootstrap
	cp src/bootstrap-silkjs .

debug:
	cd src && make -f$(MAKEFILE) debug
	cp src/silkjs .

clean:
	cd src && make -f$(MAKEFILE) clean

realclean:
	cd src && make -f$(MAKEFILE) realclean

install:
	cd src && make -f$(MAKEFILE) install

uninstall:
	cd src && make -f$(MAKEFILE) uninstall
