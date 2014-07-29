/* copyright (c) 2013 Matilda Helou <sadieperkins@riseup.net> */

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

ssize_t send_all(int socket, const uint8_t *buffer, size_t length, int flag);
ssize_t recv_all(int socket, uint8_t *buffer, size_t length, int flag);
ssize_t write_all(int fd, const uint8_t *buffer, size_t count);
int make_socket(char *node, char *port);
int make_listener(char *port);
