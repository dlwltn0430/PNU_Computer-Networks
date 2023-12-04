#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "./sock_addr" // as same as 1-s-main.c

int main() {
    int sockfd;
    struct sockaddr_un addr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the address structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("[Info] Unix socket : connected to the server\n");

    // Read input from user and send to server
    while (1) {
        printf("> Enter: ");
        memset(buffer, 0, 1024);
        if (fgets(buffer, 1024, stdin) == NULL) {
            printf("An error occurred while reading input.\n");
            break;
        }

        // Send message to the server
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("send failed");
            break;
        }

        if (strcmp(buffer, "quit\n") == 0) {
            printf("Terminate...\n: Success\n[Info] Closing socket\n");
            break;
        }
    }

    // Close the socket
    if (sockfd != -1) {
        close(sockfd);
    };

    return 0;
}
