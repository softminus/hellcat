/* copyright (c) 2014 Matilda <sadieperkins@riseup.net> */
/* this file contains functions that make dealing with the network a bit
   easier */
#include "net.h"

/* this function just runs send(2) in a loop until the entire buffer is sent or
   an error happens */

ssize_t send_all(int socket, const uint8_t *buffer, size_t length, int flag)
{
    size_t bytes_sent = 0;
    size_t bytes_unsent = length;

    ssize_t sent;

    while (bytes_sent < length)
    {
        sent = send(socket, buffer + bytes_sent, bytes_unsent, flag);
        if (sent == -1)
            return -1;
        bytes_sent += (size_t)sent;
        bytes_unsent -= (size_t)sent;
    }

    return (ssize_t)bytes_sent;
}

/* just runs recv(2) in a loop until an error happens or our peer shuts down
   the connection */

ssize_t recv_all(int socket, uint8_t *buffer, size_t length, int flag)
{
    size_t bytes_received = 0;
    size_t bytes_unreceived = length;

    ssize_t received;

    while (bytes_received < length)
    {
        received = recv(socket, buffer + bytes_received, bytes_unreceived, flag);
        if (received == -1)
            return -1;
        if (received == 0)
            return 0;
        bytes_received += (size_t)received;
        bytes_unreceived -= (size_t)received;
    }

    return (ssize_t)bytes_received;
}




ssize_t write_all(int fd, const uint8_t *buffer, size_t count)
{
    size_t bytes_sent = 0;
    size_t bytes_unsent = count;

    ssize_t sent;

    while (bytes_sent < count)
    {
        sent = write(fd, buffer + bytes_sent, bytes_unsent);
        if (sent == -1)
            return -1;
        bytes_sent += (size_t)sent;
        bytes_unsent -= (size_t)sent;
    }

    return (ssize_t)bytes_sent;
}


int make_socket(char *node, char *port) {
    int fd, rval;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* let's do fun stuff with getaddrinfo now */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* either ipv4 or ipv6, we don't care */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    rval = getaddrinfo(node, port, &hints, &result);
    if (rval != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rval));
        exit(1);
    }
    /* now we iterate over the lists of results that getaddrinfo returned
       until we can successfully make a socket and connect with it */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) {
            /* the socket making failed, so we need to do a different 
               address */
            continue;
        }

        /* we made a socket, now we try to connect */
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;  /* we successfully connected, let's exit this loop */
        }

        close(fd); /* making the socket worked but connect() failed so we 
                      close this socket */
    }
    if (rp == NULL) { /* no address worked */ 
        fprintf(stderr, "Could not connect to %s:%s\n",node, port);
        exit(1);
    }
    freeaddrinfo(result);
    return fd;
}



int make_listener(char *port)
{
    int fd, rval, i, status;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* let's do fun stuff with getaddrinfo now */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* either ipv4 or ipv6, we don't care */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;


    rval = getaddrinfo(NULL, port, &hints, &result);
    if (rval != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rval));
        exit(1);
    }
    /* now we iterate over the lists of results that getaddrinfo returned
       until we can successfully make a socket and connect with it */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1)
        {
            /* the socket making failed, so we need to do a different 
               address */
            continue;
        }

        /* we made a socket, now we try to bind  */
        if (bind(fd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            break;  /* we successfully bound, let's exit this loop */
        }

        close(fd); /* making the socket worked but bind() failed so we 
                      close this socket */
    }
    if (rp == NULL) /* no address worked */
    {
        fprintf(stderr, "Could not bind to %s\n", port);
        exit(1);
    }
    freeaddrinfo(result);
    listen(fd, 1);
    status = accept(fd, NULL, NULL);
    if(status == -1)
    {
        perror("accept");
        exit (-1);
    }

    
    return status;
}
