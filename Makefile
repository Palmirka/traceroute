CC      := cc -g
CFLAGS  := -std=gnu17 -Wall -Wextra -Werror 
LDFLAGS :=

all: traceroute

traceroute: traceroute.o printer.o utils.o

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -f *.o

tar:
	mkdir kamila_goszcz
	cp *.c *.h Makefile kamila_goszcz
	tar cfJ kamila_goszcz.tar.xz kamila_goszcz
	rm -rf kamila_goszcz

distclean: clean
	rm -rf traceroute kamila_goszcz.tar.xz kamila_goszcz