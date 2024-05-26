/*
    Kamila Goszcz 324083
*/

#include "printer.h"

void address_to_string(char* buff, struct sockaddr_in* addr){
    inet_ntop(AF_INET, &addr->sin_addr, buff, INET_ADDRSTRLEN);
}

int get_time(struct timeval* packet_time){
    int average = 0;
    for (int i = 0; i < 3; i++){
        average += packet_time[i].tv_usec + 1000000 * packet_time[i].tv_sec;
    }
    return average/3000;
}

void print_results(int packets, struct sockaddr_in* addr, char* from, struct timeval* packet_time){
    if (packets == 0) {
        printf("*\n");
    } else {
        char buff[3][20];
        for (int i = 0; i < packets; i++){
            address_to_string(buff[i], addr);
            int prt = 1;
            for (int j = 0; j < i; j++){
                prt *= strcmp(buff[i], buff[j]);
            }
            if (prt != 0)
                printf("%s ", buff[i]);
            addr += 1;
        }
        if (packets == 3)
            printf("  %dms\n", get_time(packet_time));
        else
            printf("  ???\n");
        
        if (strncmp(from, buff[0], 20) == 0){
            exit(EXIT_SUCCESS);
        }
    }
}