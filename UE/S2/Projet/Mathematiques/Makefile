ZIPFILE=projet.zip

all: build

build: clean
	mkdir .tmp
	cp -r code .tmp
	cp rapport/rapport.pdf .tmp
	cp README.md .tmp
	cd .tmp ; zip -r ../$(ZIPFILE) code rapport.pdf README.md
	rm -rf .tmp
extract:
	mkdir -p extracted ; unzip $(ZIPFILE) -d extracted

clean:
	rm -rf extracted

fclean: clean
	rm -rf $(ZIPFILE)

re: clean build
