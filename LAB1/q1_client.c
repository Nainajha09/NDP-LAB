/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 256

int main() {
    int sockfd, retval, choice, number, n;
    int arr[MAXSIZE];
    char buff[MAXSIZE];
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\nSocket creation error");
        return -1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with the server's IP address

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error\n");
        close(sockfd);
        return -1;
    }

    while (1) {
        printf("\nEnter your choice:\n");
        printf("1. Search for a number\n");
        printf("2. Sort the array (ascending or descending)\n");
        printf("3. Split the array into odd and even\n");
        printf("4. Exit\n");
        scanf("%d", &choice);
        getchar();  // To consume the newline after choice

        if (choice == 4) {
            send(sockfd, &choice, sizeof(choice), 0);
            break;
        }

        printf("Enter the number of elements in the array: ");
        scanf("%d", &n);
        printf("Enter the elements of the array:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        send(sockfd, &choice, sizeof(choice), 0);
        send(sockfd, &n, sizeof(n), 0);
        send(sockfd, arr, sizeof(int) * n, 0);

        switch (choice) {
            case 1:
                printf("Enter the number to search for: ");
                scanf("%d", &number);
                send(sockfd, &number, sizeof(number), 0);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s\n", buff);
                break;

            case 2:
                printf("Enter 1 for ascending or 2 for descending: ");
                scanf("%d", &choice);
                send(sockfd, &choice, sizeof(choice), 0);
                recv(sockfd, arr, sizeof(int) * n, 0);
                printf("Sorted array:\n");
                for (int i = 0; i < n; i++) {
                    printf("%d ", arr[i]);
                }
                printf("\n");
                break;

            case 3:
                printf("Odd numbers:\n");
                recv(sockfd, buff, sizeof(buff), 0);
                for (int i = 0; i < n; i++) {
                    if (buff[i] != '\0') {
                        printf("%d ", buff[i]);
                    }
                }
                printf("\nEven numbers:\n");
                recv(sockfd, buff, sizeof(buff), 0);
                for (int i = 0; i < n; i++) {
                    if (buff[i] != '\0') {
                        printf("%d ", buff[i]);
                    }
                }
                printf("\n");
                break;

            default:
                printf("Invalid choice\n");
                break;
        }
    }

    close(sockfd);
    return 0;
}*/
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


