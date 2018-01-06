all: test build clean

test:
	cd ./code ; ./exec1.sh -q ; ./exec2.sh -q ; ./exec3.sh -q

build: clean
	cp ./rapport/rapport.pdf code
	cd ./code ; tar -cvzf pereira-romain.tar.gz *.c customlibs rapport.pdf .bkp/*.c .bkp/customlibs README
	rm -rf code/rapport.pdf

extract:
	mkdir -p extracted
	cd extracted ; cp ../code/pereira-romain.tar.gz . ; tar -xvzf pereira-romain.tar.gz

clean:
	make -C ./code clean

fclean: clean
	rm -rf ./build
	rm -rf ./extracted
