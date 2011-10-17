CC=gcc
PROGRAMS= test1 test2

ALL: $(PROGRAMS)

test1: test1.c
	$(CC) test1.c -o test1 -L.. -lffmpeg
	
test2: test2.c
	$(CC) test2.c -o test2 -L.. -lffmpeg
	
clean:
	rm -f $(PROGRAMS)
