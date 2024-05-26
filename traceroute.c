/*
    Kamila Goszcz 324083
*/

#include "utils.h"
#include "printer.h"

struct sockaddr_in senders[MAX_PACKET];
struct timeval packet_time[MAX_PACKET];

int send_data(int sock_fd, struct sockaddr_in* recipient, int ttl, int packet){
    int pid = getpid() & 0xFF;
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = pid;
    header.icmp_hun.ih_idseq.icd_seq = pid + MAX_PACKET * ttl + packet;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum ((u_int16_t*)&header, sizeof(header));

    if (setsockopt (sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) < 0)
        return -1;

    //printf("sock_fd: %d, header %p, size_header: %ld, recipient %p, size_recipient: %ld\n", sock_fd, (void *)&header, sizeof(header), (void *)recipient, sizeof(*recipient));
    ssize_t bytes_sent = sendto (
        sock_fd,
        &header,
        sizeof(header),
        0,
        (struct sockaddr*)recipient,
        sizeof(*recipient)
    );
    if(bytes_sent < 0) {
        fprintf(stderr, "Sendto error!");
		exit(EXIT_FAILURE);
    }
    return bytes_sent;
}

int receive_data(int sock_fd, int ttl) {
    struct timeval start, current;
    gettimeofday(&start, NULL);
    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    int packet = 0;
    int pid = getpid() & 0xFF;
    while(packet < 3){
        int ready = poll (&ps, 1, MAX_WAITING_TIME);
        if (ready < 0) {
            fprintf(stderr, "Poll error!\n");
            exit(EXIT_FAILURE);
        } else if (ready == 0) {
            //printf("ready: %d \n", ps.revents);
            break;
        } else if (ps.revents == POLLIN) {

            socklen_t sender_len = sizeof(senders[packet]);
            u_int8_t buffer [IP_MAXPACKET];
            //printf("sock: %d, buff: %p, len: %d\n", sock_fd, (void*)buffer, IP_MAXPACKET);
            ssize_t packet_len = recvfrom (
                sock_fd,
                buffer,
                IP_MAXPACKET,
                0,
                (struct sockaddr*)&senders[packet],
                &sender_len
            );

            if (packet_len < 0) {
                fprintf(stderr, "Receive error!\n");
                exit(EXIT_FAILURE);
            }

            struct ip* ip_header = (struct ip*) buffer;
            u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
            struct icmp* icmp_header = (struct icmp*) icmp_packet;
            if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
                ip_header = (void *) icmp_header + 8;
                icmp_header = (void *) ip_header + 4 * ip_header->ip_hl;
		    }
            //printf("header: %d, pid: %d\n", icmp_header->icmp_hun.ih_idseq.icd_id, pid);
            if (icmp_header->icmp_hun.ih_idseq.icd_id != pid){
                continue;
            } else if(icmp_header->icmp_hun.ih_idseq.icd_seq < pid + MAX_PACKET * ttl){
                //printf("header: %d, pid: %d\n", icmp_header->icmp_hun.ih_idseq.icd_seq, MAX_TTL * MAX_WAITING_TIME * pid + MAX_PACKET * ttl);
                continue;
            } else {
                //printf("else");
                gettimeofday(&current, NULL);
                timersub(&current, &start, &packet_time[packet]);
                packet++;
            }
        }
    }
    //printf("%d\n\n", packet);
    return packet;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in recipient = check_args(argc, argv[1]);
    int sock_fd = new_socket();
    //printf("sock_fd: %d, pid: %d\n", sock_fd, getpid());
    char buff[20];
    address_to_string(buff, &recipient);
    for (int ttl = 1; ttl < MAX_TTL; ttl++){
        for (int packet = 0; packet < MAX_PACKET; packet++){
            send_data(sock_fd, &recipient, ttl, packet);
        }
        int packets = receive_data(sock_fd, ttl);
        printf("%d. ", ttl);
        print_results(packets, senders, buff, packet_time);
    }
    close(sock_fd);
    return EXIT_SUCCESS;
}