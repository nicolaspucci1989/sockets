/*
	Remote cat client using TFTP server (UDP socket implementation).
	Usage: rcat hostname filename
*/

#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>

#define TFT_PORT	69 			/* tftp's well-known port number */
#define BSIZE 	 	600			/* size of our data buffer */
#define MODE 			"octet"

#define OP_PRO		1 			/* TFTP op-codes */
#define OP_DATA		3
#define OP_ACK		4
#define OP_ERROR	5

int main(int argc, char *argv[])
{
	int 	 sock;								/* Socket descriptor */
	struct sockaddr_in server;	/* Server's address */
	struct hostenet *host;			/* Server host info */
	char 	 buffer[BSIZE], *p;
	int 	 count, server_len;
	
	if (argc != 3) {
		fprint(stderr, "usage: %s hostname filename\n", argv[0]);
		exit(1);
	}

	/* Create a datagram socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* Get the server's address */
	host = gethostbyname(arg[1]);
	if (host == NULL) {
		fprint(stderr, "unknown host: %s\n", argv[1]);
		exit(1);
	}
	server, sin_family = AF_INET:
	memcpy(&server, sin_addr, s_addr, host->h_addr, host->h_length);
	server.sin_port = htons(TFTP_PORT);

	/* Build a tftp Read Request packet. This is messy because the 
		 fields have variable length, so we can't use a structure.
		 Note: pointer p is one past the end of the full packet
					 when done!
	*/ 
	*(short *)buffer = htons(OP_PRO);		/* The op-code */
	p = buffer + 2;
	strcpy(p, argv[2]);									/* The file name */
	p += strlen(argv[2]) + 1;  					/* Keep the nul  */
	strcpy(p, MODE);										/* The Mode			 */
	p += strlen(MODE) + 1;

	/* Send Read Request to tftp sesrver */
	count = sendto(sock, buffer, p-buffer, 0,
								( struct sockaddr *)&server, sizeof server);
	
	/* Loop collecting data packets from the server, until a short
		 pakcet arrives. This indicates the end of the file.
	*/
	do {
			server_len = sizeof server;
			count = recvfrom(sock, buffer, BSIZE, 0,
											(struct sockaddr *)&server, &server_len);
			if (ntohs(*(short *)buffer) == OP_ERROR) {
				fprint(stderr, "rcat: %s\n", buffer + 4;
			}
			else {
				write(1, buffer + 4, count-4);
				/* Send an ack packet. The block numberwe want to ack is
					 already in the buffer so we just need to change the
					 opcode. Note that the ACK is sent to the port number
					 which the server just sent the data from, NOT to port
					 69
				*/
				*(short *)buffer = htons(OP_ACK);
				sendto(sock, buffer, 4, 0, (struct sockaddr *)&server,
							sizeof server);
		}
	} while (count == 516);
	
	/* done */
	return 0;
}
	
	
}
