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
    int client_fd;
    struct sockaddr_un addr;
    char buffer[256];
    
    // Create socket
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Prepare the sockaddr_un structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    
    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Connection failed");
        close(client_fd);
        exit(1);
    }
    
    // Display PID and PPID of the client process
    printf("Client PID: %d, PPID: %d\n", getpid(), getppid());
    
    // Send and receive messages to/from server in a full-duplex manner
    while (1) {
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(client_fd, buffer, strlen(buffer));
        
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            break; // Server disconnected or error
        }
        
        printf("Server: %s", buffer);
    }
    
    // Close client socket
    close(client_fd);
    return 0;
}

