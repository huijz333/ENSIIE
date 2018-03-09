CODE=code
TARBALL=pereira-romain.tar.gz

all: test build

build: clean
	cd rendu ; tar -cvzf ../$(TARBALL) code rapport/rapport.pdf README

extract:
	mkdir -p extracted ; tar -xvzf $(TARBALL) -C extracted

test:
	echo "test"

clean:
	rm -rf extracted
