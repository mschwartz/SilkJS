all:
	cd src && make
	cp src/silkjs .

osx:
	cd src && make -fMakefile.osx
	
bootstrap:
	cd src && make bootstrap
	cp src/bootstrap-silkjs .

clean:
	cd src && make clean
