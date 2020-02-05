#include "epollServer.h"

#define BACK_LOG 10
#define TRUE 1
#define CHECK_ERROR(VAL, MSG) \
    if(-1 == (VAL)) {   \
        perror(MSG);    \
        exit(EXIT_FAILURE); \
    }

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: ./epollServer port(default 8538)\n");
    }
    // create a socket instance
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_ERROR(sockfd, "socket: ");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = argc > 1 ? htons(atoi(argv[1])) : htons(8538);
    int ret = bind(sockfd, (const struct sockaddr *) &addr, sizeof(addr));
    CHECK_ERROR(ret, "bind: ");
    ret = listen(sockfd, BACK_LOG);
    CHECK_ERROR(ret, "listen: ");
    printf("Server is listening at port %s\n", argv[1]);
    char send_buf[2048] = { 0 }, recv_buf[2048] = { 0 };
    int connfd = 0;
    unsigned int len = 0u;
    struct sockaddr_in accept_addr;
    memset(&accept_addr, 0, sizeof(accept_addr));
    
    // init a epoll instance
    int epfd = epoll_create1(0);
    CHECK_ERROR(epfd, "epoll_create: ");
    // mod epoll
    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    CHECK_ERROR(ret, "epoll_ctl: ");
    // wait epoll
    struct epoll_event events[10];
    memset(events, 0, sizeof(events));
    while(TRUE) {
        printf("waiting...\n");
        ret = epoll_wait(epfd, events, 10, -1);
        CHECK_ERROR(ret, "epoll_wait: ");
        for(int i = 0; i < ret; ++i) {
            if((events[i].events & EPOLLIN) == EPOLLIN && events[i].data.fd == sockfd) {
                connfd = accept(sockfd, (struct sockaddr *) &accept_addr, (socklen_t *) &len);
                CHECK_ERROR(connfd, "accept: ");
                struct epoll_event conn_event;
                conn_event.data.fd = connfd;
                conn_event.events = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &conn_event);
                CHECK_ERROR(ret, "epoll_ctl: ");
                printf("new connection established\n");
            }
            if((events[i].events & EPOLLRDHUP) == EPOLLRDHUP && events[i].data.fd == connfd) {
                // disconnect
                printf("disconnected\n");
                goto quit;
            }
            if((events[i].events & EPOLLIN) == EPOLLIN && events[i].data.fd == connfd) {
                memset(recv_buf, 0, 2048);
                int size = recv(connfd, recv_buf, 2048, 0);
                CHECK_ERROR(size, "recv: ");
                printf("mesg recv %d byte:%s\n", size, recv_buf);
            }
        }
    }

    close(connfd);
    close(epfd);
    close(sockfd);
quit:
    return 0;
}