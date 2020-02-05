#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * EPOLL
 * 创建一颗红黑树(没有fd限制)
 * 向树上添加要监听的fd及事件(读/写)
 * 监听
 */

/**
 * int epoll_create(int size);
 * size: 大于1即可
 * 返回epoll实例的文件描述符
 */

/**
 * int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
 * epfd:
 *  树的根节点
 * op:
 *  EPOLL_CTL_ADD: epfd绑定fd, 其中fd绑定监听事件(上树)
 *  EPOLL_CTL_MOD: 更改fd的描述
 *  EPOLL_CTL_DEL: 删除fd(下树
 * typedef union epoll_data {
 *      void        *ptr;
 *      int         fd;     // 监听的文件描述符
 *      uint32_t    u32;
 *      uint64_t    u64;
 * } epoll_data_t;
 * struct epoll_event {
 *      uint32_t        events; // epoll events
 *      epoll_data_t    data;   // user data variable
 * }
 * events:
 *  EPOLLIN EPOLLOUT    读/写事件
 *  EPOLLRDHUP          
 *  EPOLLPRI
 *  EPOLLERR
 *  EPOLLHUP
 *  EPOLLET             边缘触发
 *  EPOLLONESHOT
 *  EPOLLWAKEUP
 *  EPOLLEXCLUSIVE
 * return: 成功返回0, 否则返回非0
 */

/**
 * int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
 * events:
 *  接受监听事件的返回数组, 返回数组首元素的地址
 * maxevents:
 *  数组元素的个数
 * timeout:
 *  -1, 永久监听, >0 限时等待
 * return:
 *  成功返回监听到fd的个数, 0表示timeout内无变化, -1表示出错
 */

/**
 * Epoll ET模式, 只有数据到来才触发, 不管缓存区中是否还有数据, 缓冲区剩余未读尽的数据不会导致epoll_wait返回
 * LT模式, 只要有数据都会触发, 缓冲区剩余未读尽的数据会导致epoll_wait返回
 */