/* copyright (c) 2014 Matilda Helou <sadieperkins@riseup.net> */

/* the functions in this file handle the whole "sending data out" thing */

#include "net.h"




int main(int argc, char* argv[])
{
    int datafd, controlfd;
    ssize_t rval, rval_send;
    uint8_t *buf;
    size_t bufsize;
    uint8_t cont = CONT;
    uint8_t done = DONE;

    if (argc != 3) {
        printf("usage: %s chunksize dataport controlport\nchunksize is in bytes\n", argv[0]);
        exit(10);
    }

    bufsize = atol(argv[1]);
    buf = malloc(bufsize);
    assert(buf != NULL);

    datafd = make_listener(argv[2]);
    controlfd = make_listener(argv[3]);

    while (1) {
        rval = read_all(0, buf, bufsize);
        if (rval == -1) {
            perror("read on stdin");
            exit(1);
        }

        if (rval == 0) {
            break;
        }

        rval_send = write_all(datafd, buf, rval);

        if (rval_send == -1) {
            perror("write on datafd");
            exit(1);
        }
    }

    close(datafd);
    free(buf);

    return 0;
}

