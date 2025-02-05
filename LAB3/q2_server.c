#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

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

#define MAX_BUF_SIZE 1024

// Function to swap two characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to generate and print all permutations of a string
void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
    } else {
        for (int i = l; i <= r; i++) {
            swap((str + l), (str + i));
            permute(str, l + 1, r);
            swap((str + l), (str + i)); // backtrack
        }
    }
}

int main() {
    int server_fd;
    struct sockaddr_un addr;
    char buffer[MAX_BUF_SIZE];
    socklen_t client_len;
    struct sockaddr_un client_addr;

    // Create socket
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    printf("Server is ready to receive messages\n");

    // Receive data (half-duplex, one message at a time)
    while (1) {
        client_len = sizeof(client_addr);
        memset(buffer, 0, MAX_BUF_SIZE);

        // Receive message from the client
        int bytes_received = recvfrom(server_fd, buffer, MAX_BUF_SIZE, 0, 
                                      (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received <= 0) {
            perror("Error receiving message");
            continue;
        }

        printf("Received message: %s\n", buffer);

        // Generate and print all permutations of the string
        printf("Permutations:\n");
        permute(buffer, 0, strlen(buffer) - 1);
    }

    // Close the server socket
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}

