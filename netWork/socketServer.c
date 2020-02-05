#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define LISTEN_BACKLOG 10
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: ./server port\n");
        exit(0);
    }
    // 创建socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1) {
        HANDLE_ERROR("socket: ");
    }
    // bind端口和IP信息
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port = atoi(argv[1]);
    addr.sin_port = htons(port);
    if(bind(socketfd, (struct sockaddr *) &addr, sizeof(addr))) {
        HANDLE_ERROR("bind: ");
    }
    // listen
    if(listen(socketfd, LISTEN_BACKLOG)) {
        HANDLE_ERROR("listen: ");
    }
    printf("The server is listening at port %d\n", port);
    // accept && receives
    char buf[2048] = { 0 };
    socklen_t addrlen = 0u;
    while(1) {
        struct sockaddr_in conn_addr;
        memset(&conn_addr, 0, sizeof(struct sockaddr_in));
        int connfd = accept(socketfd, (struct sockaddr *) &conn_addr, &addrlen);
        if(connfd == -1) {
            HANDLE_ERROR("accept: ");
        }
        if(-1 == send(connfd, "Welcome to Server!\n", 19, 0)) {
            HANDLE_ERROR("send: ");
        }
        while(1) {
            memset(buf, 0, 2048);
            int r = recv(connfd, buf, 2048, 0);
            if(-1 == r) {
                HANDLE_ERROR("recv: ");
            }
            if(1 == r && buf[0] == 'q' || !r) {
                break;
            }
            printf("len: %d\tval:%s\n", r, buf);  
            send(connfd, buf, r, 0);
        }
        
        close(connfd);      
    }
    close(socketfd);
    return 0;
}
