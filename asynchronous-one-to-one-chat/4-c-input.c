#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define ADDRESS "./sock_addr2" // 3-c-main.c

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
    strncpy(addr.sun_path, ADDRESS, sizeof(addr.sun_path) - 1);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type your messages below:\n");

    // Read input from user and send to server
    while (1) {
        printf("Enter: ");
        if (fgets(buffer, 1024, stdin) == NULL) {
            printf("An error occurred while reading input.\n");
            break;
        }
        
        // Send message to the server
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("send failed");
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}
