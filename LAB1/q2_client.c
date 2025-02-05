
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER];
    int n;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter a string (or 'Halt' to exit): ");
        fgets(buffer, MAX_BUFFER, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send the string to the server
        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));

        // If the user enters "Halt", terminate
        if (strcmp(buffer, "Halt") == 0) {
            printf("Client exiting...\n");
            break;
        }

        // Receive the result from the server
        n = recvfrom(sockfd, (char *)buffer, MAX_BUFFER, MSG_WAITALL, NULL, NULL);
        buffer[n] = '\0';

        // Display the result from the server
        printf("Server Response:\n%s\n", buffer);
    }

    close(sockfd);
    return 0;
}

