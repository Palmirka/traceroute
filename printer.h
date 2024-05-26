/*
    Kamila Goszcz 324083
*/

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

void address_to_string(char* buff, struct sockaddr_in* addr);
int get_time(struct timeval* packet_time);
void print_results(int packets, struct sockaddr_in* addr, char* from, struct timeval* packet_time);
