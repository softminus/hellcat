/* copyright (c) 2014 Matilda Helou <sadieperkins@riseup.net> */

/* the functions in this file handle the whole "sending data out" thing */

#include "net.h"




int main(int argc, char* argv[])
{
    int data_listener, datafd, controlfd, control_listener;
    ssize_t rval, rval_send;
    uint8_t *buf;
    size_t bufsize;
    uint8_t cont = CONT;
    uint8_t done = DONE;

    if (argc != 4) {
        printf("usage: %s chunksize dataport controlport\nchunksize is in bytes\n", argv[0]);
        exit(10);
    }

    bufsize = (size_t) atol(argv[1]);
    buf = malloc(bufsize);
    assert(buf != NULL);

    control_listener = make_listener(argv[3]);
    controlfd = accept(control_listener, NULL, NULL);
    if (controlfd == -1) {
        perror("accept on control port");
        exit(1);
    }

    data_listener = make_listener(argv[2]);
    while (1) {
        datafd = accept(data_listener, NULL, NULL);
        if (datafd == -1) {
            perror("accept on data port");
            exit(1);
        }



        rval = read_all(0, buf, bufsize);
        if (rval == -1) {
            perror("read on stdin");
            exit(1);
        }
        if (rval == 0) { /* eof on stdin */
            rval = write(controlfd, &done, 1);
            if (rval != 1) {
                perror("write on controlfd (while exiting, which makes this even more humiliating)");
                exit(1);
            }

            break;
        }

        rval_send = write_all(datafd, buf, (size_t) rval);

        if (rval_send == -1) {
            perror("write on datafd");
            exit(1);
        }

        rval = close(datafd);
        if (rval != 0) {
            perror("close");
            exit(1);
        }

        rval = write(controlfd, &cont, 1);

        if (rval != 1) {
            perror("write on controlfd");
            exit(1);
        }
    }

    printf("exiting loop\n");

    free(buf);
    close(controlfd);

    return 0;
}

