all: hellrecv hellsend

hellrecv: recv.c net.c
	clang -Wall -ggdb -Weverything -o hellrecv recv.c net.c

hellsend: send.c net.c
	clang -Wall -ggdb -Weverything -o hellsend send.c net.c

clean:
	rm -f hellsend hellrecv
