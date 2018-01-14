CODE=code
TARBALL=pereira-romain.tar.gz

all: test build clean

test:
	cd $(CODE) ; ./exec1.sh -q ; ./exec2.sh -q ; ./exec3.sh -q

build: clean
	cp ./rapport/rapport.pdf $(CODE)
	cd $(CODE) ; tar -cvzf $(TARBALL) README exo1.c exo2.c exo3.c customlibs .bkp ; rm -rf ./rapport.pdf ; mv $(TARBALL) ../

extract:
	mkdir -p extracted ; cp $(TARBALL) extracted ; cd extracted ; tar -xvzf $(TARBALL)

clean:
	make -C $(CODE) clean

fclean: clean
	rm -rf ./build
	rm -rf ./extracted
