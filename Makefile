CC=clang
CFLAGS=-Wall -ggdb -Weverything # -fsanitize=integer -fsanitize=address -fsanitize=undefined

all: hellrecv hellsend

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

hellrecv: src/recv.o src/net.o
	$(CC) $(CFLAGS) -o $@ $^

hellsend: src/send.o src/net.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f hellsend hellrecv src/*.o
