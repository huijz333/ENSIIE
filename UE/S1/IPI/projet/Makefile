SRC=./code/exo1.c ./code/exo2.c ./code/exo3.c ./rapport/rapport.pdf
LOCAL_SRC=$(notdir $(SRC))

all: test build clean

test:
	cd ./code ; ./exec1.sh -q ; ./exec2.sh -q ; ./exec3.sh -q

build: clean
	mkdir -p ./build
	mkdir -p ./build/customlibs
	cp ./rapport/rapport.pdf ./code/*.c -r ./code/customlibs/ ./build/
	cd ./build ; tar -cvzf pereira-romain.tar.gz ./*

extract:
	mkdir -p extracted
	cd extracted ; tar -xvzf ../build/pereira-romain.tar.gz

clean:
	rm -rf $(LOCAL_SRC)
	make -C ./code clean

fclean: clean
	rm -rf ./build
	rm -rf ./extracted
