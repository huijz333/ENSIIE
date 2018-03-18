TARBALL=pereira-romain.tar.gz

all: build

build: clean
	mkdir .tmp ; make -C code fclean ; cp -r code .tmp ;\
		cp rapport/rapport.pdf .tmp ; cp README .tmp;\
		cd .tmp ; tar -cvzf ../$(TARBALL) code rapport.pdf README
	rm -rf .tmp
extract:
	mkdir -p extracted ; tar -xvzf $(TARBALL) -C extracted

clean:
	rm -rf extracted
