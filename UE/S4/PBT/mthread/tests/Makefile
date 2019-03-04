# compilation
CC=gcc
CFLAG  = -Wall -g -I ../
LDFLAG = ../lib/libmthread.a -lpthread

# coloration messages
bold=$(shell (tput bold))
normal=$(shell (tput sgr0))

# liste des tests
TESTS = $(basename $(shell find . -name "*.c" -type f))

# compile tous les tests
all: mthread $(TESTS)

# recompile la bibliothèque mthread
mthread:
	make -C ../

# lance tous les tests
test: all
	@ for test in $(TESTS) ; do \
	  	echo "Lancement du test $(bold)$$test$(normal)" ; ./$$test || (echo "Echec du test $(bold)$$test$(normal)" && exit 1); \
	  done
%: %.c
	@echo "Compilation de $(bold)$^$(normal) vers $(bold)$@$(normal)"
	@$(CC) $(CFLAG) $^ -o $@ $(LDFLAG)

clean:
	rm -rf $(TESTS)

# force la recompile des tests
.PHONY: $(TESTS:=.c)
