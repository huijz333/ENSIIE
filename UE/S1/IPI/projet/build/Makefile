
SRC=../code/exo1.c ../code/exo2.c ../code/exo3.c ../rapport/rapport.pdf
LOCAL_SRC=$(notdir $(SRC))
DST=pereira-romain.tar.gz

all: test build clean

test:
	cd ../code ; ./exec1.sh -q ; ./exec2.sh -q ; ./exec3.sh -q

build:
	cp $(SRC) .
	tar -cvzf $(DST) $(LOCAL_SRC)

extract:
	tar -xvzf $(DST)

clean:
	rm -rf $(LOCAL_SRC) 

fclean: clean
	rm -rf $(DST)
