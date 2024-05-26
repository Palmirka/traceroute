/*
    Kamila Goszcz 324083
*/

#include "utils.h"

u_int16_t compute_icmp_checksum (const void *buff, int length){
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return (u_int16_t)(~(sum + (sum >> 16U)));
}

int new_socket(){
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0) {
		fprintf(stderr, "Socket error: %s\n", strerror(errno)); 
		exit(EXIT_FAILURE);
	}
    return sock_fd;
}

struct sockaddr_in check_args(int argc, char* addr){
    if (argc != 2) {
        fprintf(stderr, "Argument error! Traceroute arguments: ip_address\n");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;

    int ip_status = inet_pton(AF_INET, addr, &recipient.sin_addr);
    if (ip_status != 1){
        fprintf(stderr, "Incorrect ip_address!\n");
        exit(EXIT_FAILURE);
    }
    return recipient;
}