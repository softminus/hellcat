/* copyright (c) 2014 Matilda Helou <sadieperkins@riseup.net> */

/* the functions in this file handle the whole "receiving" thing */

#include "net.h"



int main(int argc, char* argv[])
{
    int datafd, controlfd;
    ssize_t rval, rval_send;
    uint8_t statusbyte;
    uint8_t *buf;
    size_t bufsize;

    if (argc != 5) {
        fprintf(stderr, "usage: %s chunksize host dataport controlport\nchunksize is in bytes\n", argv[0]);
        exit(10);
    }
    bufsize = (size_t) atol(argv[1]);
    buf = malloc(bufsize);
    assert(buf != NULL);

    controlfd = make_socket(argv[2], argv[4]);
    sleep(1);

    while (1) {
        rval = read(controlfd, &statusbyte, 1);

        if (rval != 1) {
            perror("read on controlfd");
            exit(1);
        }

        if (statusbyte == DONE) {
            fprintf(stderr, "received DONE\n");
            break;
        }

        assert (statusbyte == CONT);

        datafd = make_socket(argv[2], argv[3]);
        rval = read_all(datafd, buf, bufsize);

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

