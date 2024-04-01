#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 6668
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buf[BUF_SIZE];

    // Crear socket del cliente
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    //printf("Conectado al servidor.\n");

    std::string message;
    std::getline(std::cin, message);
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    std::cout << "Mensaje enviado al servidor: " << message << std::endl;

    // Recibir respuesta del servidor
    int bytesReceived = recv(clientSocket, buf, BUF_SIZE, 0);
    if (bytesReceived == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buf[bytesReceived] = '\0'; // Añadir terminador nulo al final del mensaje recibido
    std::cout << "Respuesta del servidor: " << buf << std::endl;

    // Aquí puedes enviar y recibir datos a través del primer cliente

    // Cerrar el socket del cliente
    close(clientSocket);

    return 0;
}

