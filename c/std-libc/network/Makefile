


all:


%.o: %.c
	$(CC) -std=c99 -c -o $@ $^

test: main.o
	$(CC) -o $@ $<


all: test



clean:
	rm -rf *.o


.PHONY:all clean

.FORCE: clean