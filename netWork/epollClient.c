#include "epollClient.h"

#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)


int main(int argc, char* argv[]) {
    
    // client socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd) {
        HANDLE_ERROR("socket: ");
    }
    //  connect
    char* ip = NULL; 
    int port = 0;
    if(argc < 3) {
        printf("The default IP Address is 127.0.0.1:8538\n");
        ip = "127.0.0.1";
        port = 8538;
    } else if(argc == 3) {
        printf("The IP Address is: %s\nThe Port is: %s\n", argv[1], argv[2]);
        ip = argv[1];
        port = atoi(argv[2]);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if(connect(clientfd, (const struct sockaddr *) &addr, sizeof(addr))) {
        HANDLE_ERROR("connect: ");
    }
    // send || recv 
    char buf[2048] = { 0 };
    char recvBuf[2048] = { 0 };
    while(1) { 
        memset(buf, 0, 2048);
        memset(recvBuf, 0, 2048);
        scanf("%s", buf);
        if(-1 == send(clientfd, buf, strlen(buf), 0)) {
            HANDLE_ERROR("send: ");
        }
        printf("send msg %ld bytes:%s\n", strlen(buf), buf);        
    }
    close(clientfd);
    return 0;
}