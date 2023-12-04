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

#define INET_PORT 1234        // as same as 3-c-main.c
#define UNIX_SOCKET_PATH "./sock_addr" // as same as 2-s-input.c

int main() {
    int unix_sockfd, inet_sockfd, len;
    struct sockaddr_un unix_addr;
    struct sockaddr_in server_addr;
    socklen_t unix_addr_len = sizeof(unix_addr);
    socklen_t inet_addr_len = sizeof(server_addr);
    char buffer[1024];

    /* communicate with terminal 2 (AF_UNIX) */
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

    // wait until make a successful connection with client(terminal 2)
    int new_unix_sockfd = -1;
    while (new_unix_sockfd < 0) {
        new_unix_sockfd = accept(unix_sockfd, (struct sockaddr *)&unix_addr, &unix_addr_len);
        if (new_unix_sockfd < 0) {
            perror("accept error");
        }
    }

    printf("[Info] Unix socket : client connected\n");

    /* communicate with terminal 3 (AF_INET) */
    // create
    inet_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (inet_sockfd < 0) {
        perror("AF_INET socket creation failed");
        exit(EXIT_FAILURE);
    }

    // set socket options
    int opt = 1;
    if (setsockopt(inet_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(INET_PORT);
    len = sizeof(struct sockaddr_in);

    // bind
    if (bind(inet_sockfd, (struct sockaddr *)&server_addr, len) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    
    // listen
    if (listen(inet_sockfd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    
    printf("[Info] Inet socket : waiting for conn req\n");

    // wait until make a successful connection with client(terminal 3)
    int new_inet_sockfd = -1;
    while (new_inet_sockfd <= 0) {
        new_inet_sockfd = accept(inet_sockfd, (struct sockaddr *)&server_addr, &inet_addr_len);
        if (new_inet_sockfd < 0) {
            perror("accept error");
        }
    }

    printf("[Info] Inet socket : client connected\n");

    // non-blocking mode
    int flags1 = fcntl(unix_sockfd, F_GETFL, 0);
    fcntl(unix_sockfd, F_SETFL, flags1 | O_NONBLOCK);

    int flags2 = fcntl(new_unix_sockfd, F_GETFL, 0);
    fcntl(new_unix_sockfd, F_SETFL, flags2 | O_NONBLOCK);

    int flags3 = fcntl(inet_sockfd, F_GETFL, 0);
    fcntl(inet_sockfd, F_SETFL, flags3 | O_NONBLOCK);

    int flags4 = fcntl(new_inet_sockfd, F_GETFL, 0);
    fcntl(new_inet_sockfd, F_SETFL, flags4 | O_NONBLOCK);
    
    // recv
    while (1) {
        // receive from terminal 2
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_from_2 = recv(new_unix_sockfd, buffer, sizeof(buffer), 0);
        if (recv_from_2 > 0) {
            printf("[ME] : %s", buffer);
            
            // send to terminal 3
            if (inet_sockfd != -1 && new_inet_sockfd != -1) {
                send(new_inet_sockfd, buffer, sizeof(buffer), 0);
            }

            // quit
            if (strcmp(buffer, "quit\n") == 0) {
                if (inet_sockfd != -1 && new_inet_sockfd != -1) {
                    printf("[Server] quit\n");
                    close(inet_sockfd);
                    close(new_inet_sockfd);
                }
                inet_sockfd = -1;
                new_inet_sockfd = -1;

                printf("[Info] Closing sockets\n");

                break;
            }
        } 
        
        // receive from terminal 3
        if (inet_sockfd != -1 && new_inet_sockfd != -1) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t recv_from_3 = recv(new_inet_sockfd, buffer, sizeof(buffer), 0);
            if (recv_from_3 > 0) {
                printf("[YOU] : %s", buffer);
            
                // quit
                if (strcmp(buffer, "quit\n") == 0) {
                    close(inet_sockfd);
                    close(new_inet_sockfd);

                    inet_sockfd = -1;
                    new_inet_sockfd = -1;

                    printf("[Info] Closing socket\n");
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

    //unlink(UNIX_SOCKET_PATH); // delete an existing socket file

    return 0;
}
