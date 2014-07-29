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
        printf("usage: %s host dataport controlport\n", argv[0]);
        exit(10);
    }
    controlfd = make_socket(argv[1], argv[3]);

    while (1) {
        datafd = make_socket(argv[1], argv[2]);
        rval = read_all(datafd, buf, BUFSIZE);

        if (rval == -1) {
            perror("read on datafd");
            exit(1);
        }

        if (rval == 0) { /* eof on datafd */
            close(datafd);
            break;
        }

        rval_send = write_all(1, buf, rval);

        if (rval_send == -1) {
            perror("write on datafd");
            exit(1);
        }

        close(datafd);

        /* see if we have more shit to read */

        rval = read(controlfd, &statusbyte, 1);

        if (rval != 1) {
            perror("read on controlfd");
            exit(1);
        }

        if (statusbyte == DONE) {
            break;
        }

        assert (statusbyte == CONT);
    }

    close(controlfd);

    return 0;
}

