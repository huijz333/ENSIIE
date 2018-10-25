help:
	@echo "Compile le programme de test"
	@echo "> make a.out"
	@echo ""
	@echo "Compile en bibliothèque dynamique"
	@echo "> make shared"

a.out: main.c hp_allocator.c hp_allocator.h
	gcc $^ -o $@ -g

shared: hp_allocator.c hp_allocator.h
	gcc -c -fpic hp_allocator.c -o hp_allocator.o
	gcc -shared -o libhp_allocator.so hp_allocator.o

clean:
	-rm *.o
	-rm libhp_allocator.so
	-rm a.out
