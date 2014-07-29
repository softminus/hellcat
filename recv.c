/* copyright (c) 2014 Matilda Helou <sadieperkins@riseup.net> */

/* the functions in this file handle the whole "receiving" thing */

#include "net.h"

#define BUFSIZE 2048


int main(int argc, char* argv[])
{
    int datafd, controlfd;
    ssize_t rval, rval_send;
    uint8_t buf[BUFSIZE];
    uint8_t statusbyte;

    if (argc != 4) {
        fprintf(stderr, "usage: %s host dataport controlport\n", argv[0]);
        exit(10);
    }
    controlfd = make_socket(argv[1], argv[3]);
    sleep(1);

    while (1) {
        rval = read(controlfd, &statusbyte, 1);

        if (rval != 1) {
            perror("read on controlfd");
            exit(1);
        }

        if (statusbyte == DONE) {
            break;
        }

        assert (statusbyte == CONT);

        datafd = make_socket(argv[1], argv[2]);
        rval = read_all(datafd, buf, BUFSIZE);

        if (rval == -1) {
            perror("read on datafd");
            exit(1);
        }

        if (rval == 0) { /* eof on datafd */
            fprintf(stderr, "unexpected EOF on datafd\n");
            exit(1);
        }

        rval_send = write_all(1, buf, (size_t)rval);

        if (rval_send != rval) {
            perror("write on stdout");
            exit(1);
        }

        close(datafd);
    }

    close(controlfd);

    return 0;
}

