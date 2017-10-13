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
    char buffer[256]; // Recibir mensajes del servidor

    if (argc < 3) { // Chequeo de argumentos
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }


    portno = atoi(argv[2]); // Pasar cadena a entero, asci to integer
    // CREAR SOCKET
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // en sockfd deja el descriptor de archivo, seria 3
    if (sockfd < 0) // Para comprobar el socket
        error("ERROR opening socket");


    server = gethostbyname(argv[1]); // Consulta al dns y contesta con el numero ip
    // Queda la direccion del server en formato numero ip
		// gethostbyname devuelve un puntero a un hostent
    if (server == NULL) { // Comprobar el puntero
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr)); // Setear todos los valores del buffer a cero.
    serv_addr.sin_family = AF_INET; // Protocolo TCP
    bcopy((char *)server->h_addr, // completar mas campos
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


		/* Conexion */
    if (connect(sockfd, &serv_addr,sizeof(serv_addr)) < 0) // CONEXION manda el datagrama el socket remo
        error("ERROR connecting");


    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer)); // escribir el buffer al socket n ENVIAR
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255); // Leer la info del socket al buffer READ
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer); // Imprimir lo recibido

    return 0;
}
