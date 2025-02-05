#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCKET_PATH "/tmp/chat_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[256];
    pid_t pid;
    
    // Create socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    
    // Unlink the socket file if it exists
    unlink(SOCKET_PATH);
    
    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }
    
    // Listen for incoming connections (max 1 client)
    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }
    
    printf("Server ready to accept connections\n");
    
    // Accept the client connection
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }
    
    // Display PID and PPID of the server process
    printf("Server PID: %d, PPID: %d\n", getpid(), getppid());
    
    // Fork to handle bidirectional communication
    pid = fork();
    
    if (pid == 0) { // Child process (communication with client)
        // Close server socket in child process
        close(server_fd);
        
        // Receive messages from client and send response back
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int n = read(client_fd, buffer, sizeof(buffer) - 1);
            if (n <= 0) {
                break; // No more data or error
            }
            
            printf("Client: %s", buffer);
            
            // Send message back to client
            printf("Server: ");
            fgets(buffer, sizeof(buffer), stdin);
            write(client_fd, buffer, strlen(buffer));
        }
        
        close(client_fd);
        exit(0);
    } else { // Parent process
        // Close client socket in parent process
        close(client_fd);
        
        // Wait for child process to finish
        wait(NULL);
    }

    // Clean up and exit
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}

