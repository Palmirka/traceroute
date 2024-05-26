/*
    Kamila Goszcz 324083
*/

#include <arpa/inet.h>
#include <sys/poll.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_TTL 30
#define MAX_PACKET 3
#define MAX_WAITING_TIME 1000

u_int16_t compute_icmp_checksum (const void *buff, int length);
int new_socket(void);
struct sockaddr_in check_args(int argc, char* addr);
