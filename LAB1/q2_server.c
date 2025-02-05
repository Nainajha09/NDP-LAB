
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER 1024

// Function to check if a string is a palindrome
int is_palindrome(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Not a palindrome
        }
    }
    return 1; // Palindrome
}

// Function to count vowels in a string
void count_vowels(char* str, int* vowels_count) {
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = str[i];
        if (ch == 'a' || ch == 'A') vowels_count[0]++;
        else if (ch == 'e' || ch == 'E') vowels_count[1]++;
        else if (ch == 'i' || ch == 'I') vowels_count[2]++;
        else if (ch == 'o' || ch == 'O') vowels_count[3]++;
        else if (ch == 'u' || ch == 'U') vowels_count[4]++;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_BUFFER];
    int len, n;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    while (1) {
        len = sizeof(client_addr);
        n = recvfrom(sockfd, (char *)buffer, MAX_BUFFER, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        // If the client sends "Halt", break the loop
        if (strcmp(buffer, "Halt") == 0) {
            printf("Server shutting down.\n");
            break;
        }

        // Process the string: Check palindrome and count vowels
        int vowels_count[5] = {0}; // A, E, I, O, U
        int palindrome = is_palindrome(buffer);
        count_vowels(buffer, vowels_count);

        int str_length = strlen(buffer);
        char result[MAX_BUFFER];
        snprintf(result, sizeof(result), 
                 "Palindrome: %s\nLength: %d\nA: %d, E: %d, I: %d, O: %d, U: %d\n", 
                 palindrome ? "Yes" : "No", str_length, vowels_count[0], vowels_count[1], 
                 vowels_count[2], vowels_count[3], vowels_count[4]);

        // Send the result back to the client
        sendto(sockfd, result, strlen(result), MSG_CONFIRM, (const struct sockaddr *)&client_addr, len);
    }

    close(sockfd);
    return 0;
}

