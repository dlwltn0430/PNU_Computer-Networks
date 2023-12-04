#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#define INET_PORT 1234        // as same as 1-c-main.c
#define SERVER_IP "127.0.0.1" // as same as 1-c-main.c
#define UNIX_SOCKET_PATH "./sock_addr2" // as same as 4-s-input.c

int main() {
    int unix_sockfd, inet_sockfd, len;
    struct sockaddr_un unix_addr;
    struct sockaddr_in server_addr;
    socklen_t unix_addr_len = sizeof(unix_addr);
    socklen_t inet_addr_len = sizeof(server_addr);
    char buffer[1024];

    /* communicate with terminal 4 (AF_UNIX) */
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

    printf("[Info] Unix socket : waiting for conn req\n");

    // wait until make a successful connection with client(terminal 4)
    int new_unix_sockfd = -1;
    while (new_unix_sockfd < 0) {
        new_unix_sockfd = accept(unix_sockfd, (struct sockaddr *)&unix_addr, &unix_addr_len);
        if (new_unix_sockfd < 0) {
            perror("accept error");
        }
    }

    printf("[Info] Unix socket : client connected\n");

    /* communicate with terminal 1 (AF_INET) */
    // create
    inet_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (inet_sockfd < 0) {
        perror("AF_INET socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(INET_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // connect
    if (connect(inet_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("INET connect failed");
        exit(EXIT_FAILURE);
    }

    printf("[Info] Inet socket : connected to the server\n");

    // non-blocking mode
    int flags1 = fcntl(unix_sockfd, F_GETFL, 0);
    fcntl(unix_sockfd, F_SETFL, flags1 | O_NONBLOCK);

    int flags2 = fcntl(new_unix_sockfd, F_GETFL, 0);
    fcntl(new_unix_sockfd, F_SETFL, flags2 | O_NONBLOCK);

    int flags3 = fcntl(inet_sockfd, F_GETFL, 0);
    fcntl(inet_sockfd, F_SETFL, flags3 | O_NONBLOCK);
    
    // recv
    while (1) {
        // receive from terminal 4
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_from_4 = recv(new_unix_sockfd, buffer, sizeof(buffer), 0);
        if (recv_from_4 > 0) {
            printf("[ME] : %s", buffer);
            
            // send to terminal 1
            if (inet_sockfd != -1) {
                send(inet_sockfd, buffer, sizeof(buffer), 0);
            }

            // quit
            if (strcmp(buffer, "quit\n") == 0) {
                if (inet_sockfd != -1) {
                    close(inet_sockfd);
                }
                inet_sockfd = -1;
                break;
            }
        } 
        
        // receive from terminal 1
        if (inet_sockfd != -1) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t recv_from_1 = recv(inet_sockfd, buffer, sizeof(buffer), 0);
            if (recv_from_1 > 0) {
                printf("[YOU] : %s", buffer);
            
                // quit
                if (strcmp(buffer, "quit\n") == 0) {
                    close(inet_sockfd);
                    inet_sockfd = -1;
                }
            }
        }
    }
    
    if (unix_sockfd != -1) {
        close(unix_sockfd); 
    }
    
    if (new_unix_sockfd != -1) {
        close(new_unix_sockfd); 
    }

    return 0;
}
