#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
		/* Esta funcion se llama cuando falla un llamado al sistema */
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
		/* La variable server es un puntero a una estructura tipo
			 hostent. Define a un host en internet */
    struct hostent *server;
    char buffer[256];

    if (argc < 3) { // Chequeo de argumentos
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }


    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");


    server = gethostbyname(argv[1]);
		// gethostbyname devuelve un puntero a un hostent
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


		// Setear todos los valores del buffer a cero.
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


		/* Conexion */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
