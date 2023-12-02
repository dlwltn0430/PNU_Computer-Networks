#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>

#define INET_PORT 8080        // 1-s-main.c
#define SERVER_IP "127.0.0.1" // 1-s-main.c
#define UNIX_SOCKET_PATH "./sock_addr2" // 4-c-input.c

int main() {
    int inet_sockfd, unix_sockfd, len;
    //struct sockaddr_in server_addr;
    struct sockaddr_un unix_addr;
    char buffer[1024];

    /*
    // create
    inet_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (inet_sockfd < 0) {
        perror("AF_INET socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(INET_PORT);

    // connect
    if (connect(inet_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("INET connect failed");
        close(inet_sockfd);
        exit(EXIT_FAILURE);
    }
    */

    // create
    unix_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unix_sockfd < 0) {
        perror("AF_UNIX socket creation failed");
        exit(EXIT_FAILURE);
    }

    // set an address
    memset(&unix_addr, 0, sizeof(struct sockaddr_un));
    unix_addr.sun_family = AF_UNIX;
    strcpy(unix_addr.sun_path, UNIX_SOCKET_PATH);
    len = strlen(unix_addr.sun_path) + sizeof(unix_addr.sun_family);

    unlink(UNIX_SOCKET_PATH); // delete an existing socket file

    // bind
    if (bind(unix_sockfd, (struct sockaddr *)&unix_addr, len) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    
    // listen
    if (listen(unix_sockfd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    
    // recv
    while (1) {
        /*
        memset(buffer, 0, sizeof(buffer));
        ssize_t read_bytes = read(inet_sockfd, buffer, sizeof(buffer));
        if (read_bytes > 0) {
            printf("[YOU] %s", buffer);
        }
        */

        int unix_sock = accept(unix_sockfd, NULL, NULL);
        /*
        if (unix_sock < 0) {
            // Non-blocking mode error handling
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("AF_UNIX accept failed");
            }
        } else {
            */
            memset(buffer, 0, sizeof(buffer));
            //ssize_t read_bytes = read(unix_sockfd, buffer, sizeof(buffer));
            if (read(unix_sock, buffer, sizeof(buffer)) == -1) {
                perror("read error");
                exit(1);
            }

            printf("[ME] %s", buffer);
        //}
    }

    //close(inet_sockfd);
    close(unix_sockfd);
    return 0;
}

