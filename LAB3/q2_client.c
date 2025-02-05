#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define MAX_BUF_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[MAX_BUF_SIZE];

    // Create socket
    if ((client_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Send a message to the server (half-duplex: send one message at a time)
    while (1) {
        printf("Enter a string to send to the server (or type 'exit' to quit): ");
        fgets(buffer, MAX_BUF_SIZE, stdin);

        // Remove newline character from the string
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Send message to the server
        if (sendto(client_fd, buffer, strlen(buffer), 0, 
                   (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
            perror("Error sending message");
            continue;
        }

        printf("Message sent to server: %s\n", buffer);
    }

    // Close the client socket
    close(client_fd);
    return 0;
}

