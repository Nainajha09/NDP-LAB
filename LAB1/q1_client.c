
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 256

int main() {
    char buff[MAXSIZE];
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    int choice, n, number;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\nSocket creation error");
        return -1;
    }

    // Set server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error");
        return -1;
    }

    while (1) {
        printf("Enter choice (1 = search, 2 = sort, 3 = split, 4 = exit): ");
        scanf("%d", &choice);

        if (choice == 4) {
            send(sockfd, "4", 1, 0);
            break;
        }

        // Send choice
        sprintf(buff, "%d", choice);
        send(sockfd, buff, strlen(buff), 0);

        // Send array size
        printf("Enter number of elements in array: ");
        scanf("%d", &n);
        send(sockfd, &n, sizeof(n), 0);

        // Send array elements
        int arr[n];
        printf("Enter the elements:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
        send(sockfd, arr, sizeof(arr), 0);

        if (choice == 1) {
            // Search operation
            printf("Enter number to search: ");
            scanf("%d", &number);
            send(sockfd, &number, sizeof(number), 0);
        } else if (choice == 2) {
            // Sort operation
            printf("Enter 1 for Ascending or 2 for Descending: ");
            scanf("%d", &choice);
            send(sockfd, &choice, sizeof(choice), 0);
        }

        // Receive result from server
        int recv_bytes = recv(sockfd, buff, sizeof(buff), 0);
        printf("Received from server: %s\n", buff);
    }

    close(sockfd);
    return 0;
}


