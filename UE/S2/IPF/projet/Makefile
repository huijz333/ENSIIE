TARBALL=pereira-romain.tar.gz

all: build

build: clean
	mkdir .tmp ; make -C code fclean ; cp -r code .tmp ;\
		cp rapport/rapport.pdf .tmp ; cp README .tmp;\
		cp out.csv .tmp ; cd .tmp ; \
		tar -cvzf ../$(TARBALL) code rapport.pdf README out.csv
	rm -rf .tmp
extract:
	mkdir -p extracted ; tar -xvzf $(TARBALL) -C extracted

clean:
	rm -rf extracted
