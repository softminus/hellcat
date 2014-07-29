CC=clang
CFLAGS=-Wall -ggdb -Weverything

all: hellrecv hellsend

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

hellrecv: src/recv.o src/net.o
	$(CC) -o $@ $^

hellsend: src/send.o src/net.o
	$(CC) -o $@ $^

clean:
	rm -f hellsend hellrecv src/*.o
