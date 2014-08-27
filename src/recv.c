/* copyright (c) 2014 Matilda Helou <sadieperkins@riseup.net> */

/* the functions in this file handle the whole "receiving" thing */

#include "net.h"
#include <sys/time.h>



int main(int argc, char* argv[])
{
    int datafd, controlfd, control_listener, data_listener, listening;
    ssize_t rval, rval_send;
    uint8_t statusbyte;
    uint8_t *buf;
    size_t bufsize, totalsize = 0;
    struct timeval start, end;
    float elapsed;

    if ((argc != 5) || (argc != 4)) {
        fprintf(stderr, "usage: %s chunksize dataport controlport [host]\nchunksize is in bytes\n", argv[0]);
        exit(10);
    }

    if (argc == 4) {
        listening = 1;
    } else {
        listening = 0;
    }

    bufsize = (size_t) atol(argv[1]);
    buf = malloc(bufsize);
    assert(buf != NULL);

    if (listening == 0) {
        controlfd = make_socket(argv[4], argv[3]);
    } else {
        control_listener = make_listener(argv[3]);
        data_listener = make_listener(argv[2]);
        controlfd = accept(control_listener, NULL, NULL);
        if (controlfd == -1) {
            perror("accept on control port");
            exit(1);
        }
    }

    sleep(1);
    gettimeofday(&start, NULL);

    while (1) {
        rval = read(controlfd, &statusbyte, 1);

        if (rval != 1) {
            perror("read on controlfd");
            exit(1);
        }

        if (statusbyte == DONE) {
            fprintf(stderr, "received DONE\n");
            gettimeofday(&end, NULL);
            elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000;
            fprintf(stderr, "transferred %zu bytes in %.3f seconds\n", totalsize, elapsed);

            break;
        }

        assert (statusbyte == CONT);

        if (listening == 0) {
            datafd = make_socket(argv[2], argv[3]);
        } else {
            datafd = accept(data_listener, NULL, NULL);
            if (datafd == -1) {
                perror("accept on data port");
                exit(1);
            }
        }

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

        totalsize += (size_t) rval;
        close(datafd);
    }

    close(controlfd);

    return 0;
}

