#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "discovery.h"

void
check(int status, char *error_payload)
{
	if (status == -1) {
		status = errno;
		perror(error_payload);
		exit(status);
	}
}

int
main(int argc, char *argv[])
{
	const int yes = 1;				// for socket options
	const int buffer_size = 256;

	int sockid, status;
	unsigned char buffer[buffer_size];
	char address[INET_ADDRSTRLEN];
	struct addrinfo hints;
	struct addrinfo *destination;
	struct sockaddr_storage source;
	struct sockaddr_in *source_info;
	struct discovery *packet;

	socklen_t source_size = sizeof(source);
	uint8_t payload[] = {0x01, 0x00, 0x00, 0x00};	// discovery payload

	bzero(&hints, sizeof(hints));			// zero out hints struct
	hints.ai_family = AF_INET;			// use ip version 4
	hints.ai_socktype = SOCK_DGRAM;			// use udp
	hints.ai_protocol = IPPROTO_UDP;		// no seriously, use udp
	hints.ai_flags = AI_PASSIVE;			// detect my ip address

	getaddrinfo("255.255.255.255", "10001", &hints, &destination);

	sockid = socket(destination->ai_family,
			destination->ai_socktype, destination->ai_protocol);
	check(sockid, "Couldn't create socket");

	status = setsockopt(sockid, SOL_SOCKET,
			SO_BROADCAST, &yes, sizeof(yes));
	check(status, "Couldn't enable broadcast");

	status = sendto(sockid, payload, sizeof(payload), 0,
			destination->ai_addr, destination->ai_addrlen);

	check(status, "Couldn't send payload");
	printf("Bytes sent: %d\n", status);

	status = recvfrom(sockid, buffer, buffer_size, 0,
			(struct sockaddr *) &source, &source_size);

	check(status, "Couldn't receive payload");
	printf("Bytes received: %d\n", status);

	source_info = (struct sockaddr_in *) &source;
	printf("%s\n", inet_ntop(source_info->sin_family,
				&(source_info->sin_addr),
				address, INET_ADDRSTRLEN));

	packet = discovery_new_from(buffer);
	if (packet == NULL) {
		printf("Couldn't parse discovery packet\n");
		return -1;
	}


	discovery_free(packet);

	status = close(sockid);
	check(status, "Couldn't close socket");
	freeaddrinfo(destination);

	return status;
}
