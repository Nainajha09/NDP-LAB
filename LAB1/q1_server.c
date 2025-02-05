#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 256


void sort_array(int arr[], int n, int ascending) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((ascending && arr[i] > arr[j]) || (!ascending && arr[i] < arr[j])) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}


void split_array(int arr[], int n, int *odd_arr, int *even_arr, int *odd_count, int *even_count) {
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            even_arr[(*even_count)++] = arr[i];
        } else {
            odd_arr[(*odd_count)++] = arr[i];
        }
    }
}

int main() {
    int sockfd, newsockfd, retval;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    int arr[MAXSIZE], result_arr[MAXSIZE], n, choice, number, found;
    int odd_arr[MAXSIZE], even_arr[MAXSIZE], odd_count, even_count;

 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\nSocket creation error");
        return -1;
    }


    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  
    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Binding error");
        close(sockfd);
        return -1;
    }


    retval = listen(sockfd, 1);
    if (retval == -1) {
        printf("Listen error");
        close(sockfd);
        return -1;
    }

    // Accept a connection from the client
    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &addrlen);
    if (newsockfd == -1) {
        printf("Accept error");
        close(sockfd);
        return -1;
    }

    printf("Server connected to client\n");

    while (1) {
        memset(buff, '\0', sizeof(buff));

        // Receive the choice from the client (operation type)
        int recv_bytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recv_bytes == -1) {
            printf("Error receiving data from client\n");
            break;
        }

        sscanf(buff, "%d", &choice);  // The first value in the message is the choice
        if (choice == 4) {
            printf("Server exiting.\n");
            break;
        }

        // Receiving the array size and the integers
        recv_bytes = recv(newsockfd, &n, sizeof(n), 0);
        if (recv_bytes == -1) {
            printf("Error receiving array size\n");
            break;
        }

        recv_bytes = recv(newsockfd, arr, sizeof(int) * n, 0);
        if (recv_bytes == -1) {
            printf("Error receiving array data\n");
            break;
        }

        // Process based on client choice
        switch (choice) {
            case 1:  // Search for a number
                recv_bytes = recv(newsockfd, &number, sizeof(number), 0);
                found = 0;
                for (int i = 0; i < n; i++) {
                    if (arr[i] == number) {
                        found = 1;
                        break;
                    }
                }
                if (found) {
                    strcpy(buff, "Number found");
                } else {
                    strcpy(buff, "Number not found");
                }
                send(newsockfd, buff, strlen(buff), 0);
                break;

            case 2:  // Sort the array (ascending or descending)
                recv_bytes = recv(newsockfd, &choice, sizeof(choice), 0);  // Ascending (1) or Descending (2)
                if (recv_bytes == -1) {
                    printf("Error receiving sort choice\n");
                    break;
                }
                sort_array(arr, n, choice == 1 ? 1 : 0);
                send(newsockfd, arr, sizeof(int) * n, 0);
                break;

            case 3:  // Split the array into odd and even
                odd_count = 0;
                even_count = 0;
                split_array(arr, n, odd_arr, even_arr, &odd_count, &even_count);
                
                // Send odd numbers to client
                send(newsockfd, &odd_count, sizeof(odd_count), 0);
                send(newsockfd, odd_arr, sizeof(int) * odd_count, 0);

                // Send even numbers to client
                send(newsockfd, &even_count, sizeof(even_count), 0);
                send(newsockfd, even_arr, sizeof(int) * even_count, 0);
                break;

            default:
                strcpy(buff, "Invalid choice");
                send(newsockfd, buff, strlen(buff), 0);
                break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

