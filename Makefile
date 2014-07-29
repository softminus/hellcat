all: recv send 

recv: recv.c net.c
	clang -Wall -ggdb -Weverything -o hellrecv recv.c net.c

send: send.c net.c
	clang -Wall -ggdb -Weverything -o hellsend send.c net.c

clean:
	rm -f hellsend hellrecv
