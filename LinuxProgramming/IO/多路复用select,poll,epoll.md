# 多路复用select,pool,epool
>I/O多路复用(又被称为“事件驱动”)，它是一种机制，可以监视多个文件描述符，一旦某个文件描述符就绪，就可以通知应用程序进行相应的读写操作；

> **IO多路复用适用如下场合:**
> - 当客户处理多个描述字时（一般是交互式输入和网络套接口），必须使用I/O复用
> - 如果一个TCP服务器既要处理监听套接口，又要处理已连接套接口，一般也要用到I/O复用
> - 如果一个服务器即要处理TCP，又要处理UDP，一般要使用I/O复用
> - 如果一个服务器要处理多个服务或多个协议，一般要使用I/O复用

> **注意**
> - 在使用select,poll,epoll监控文件的时候，如果从用户空间关闭了文件描述符，这样有可能导致导致BUG，注意

#### select
> select和pselect允许进程监听多个文件描述符，直到一个或多文件描述符IO操作就绪后就返回通知应用程序，最多能监听FD_SETSIZE=1024个文件描述，该值硬编码再源码中，是不可以修改的,监听文件描述符大于1024的场景时需要使用poll替换select;

- `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);`
  > `#include <sys/select.h>`
  >
  > `#include <sys/time.h>`
  >
  > 描述：程序会阻塞在select，当被监听的文件集合发生事件时，才会立即返回；
  >
  > ndfs：被设置为三个文件集合中的最大描述符 + 1；这样就能保证每个文件描述符都被检查到了，被检测的文件描述符从0, 1, 2, ..., nfds - 1;
  >
  > readfds: 用于设置需要监听可读操作文件集合
  >
  > writefds: 用来设置需要监听可写操作的文件集合
  >
  > exceptfds: 用来设置需要监听异常的文件集合
  >
  > timeout: 设置select的阻塞超时时间
  >
  > 返回：成功-返回监听文件集的总个数，当超时时返回0；失败-返回-1
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/select.2.html)
  >
  > noet: 会用到下面的这几个宏定义来设置文件集合
  > - void FD_CLR(int fd, fd_set *set); /* 将某个描述符从监听集中删除 */
  > - int  FD_ISSET(int fd, fd_set *set); /* 判断某个描述符是否发生了事件 */
  > - void FD_SET(int fd, fd_set *set); /* 将某个描述符加入某个监听集合中 */
  > - void FD_ZERO(fd_set *set); /* 清空监听集 */

> select缺点：
> - 最多监听1024个文件描述符
> - 每次调用select，都需要将用户空间的监控文件集数据结构复制到内核空间
> - 每次select返回，都需要便利所有文件描述符才能够知道那个描述符发生了事件
> - select的事件时水平触发的，事件处理后需要清空文件集后再重新设置

- `int poll(struct pollfd *fds, nfds_t nfds, int timeout);`
  > `#include <poll.h>`
  >
  > 描述：poll和select功能一致，只是没有了监控文件描述符数量的限制，使用数组的方式存储需要监控的文件描述符
  >
  > fds: 文件描述符集合
  >
  > ngds: 指定文件描述符的数量
  >
  > timeout: 设置超时时间，时间单位为毫秒(ms), 小于0时，代表poll函数一致阻塞到由事件发生，等于0时，poll函数会立即返回，即使没有事件发生，大于0时，表示等待事件的超时时间
  >
  > 返回：成功-返回正数，代表发生事件的描述符数量，返回0代表等待时间超时，文件描述符没有事件发生；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/poll.2.html)
  >
  > pool函数调用返回的三个条件：
  > - 监听的描述符有事件发生
  > - 被信号中断
  > - 超时返回
```C
struct pollfd {
    int   fd;         /* 指定监听的文件描述符 */
    short events;     /* 用于指定文件描述符需要监听的事件，poll只有监听到该事件时才返回，如果设置为0时，只有文件描述符发生POLLHUP, POLLERR， POLLNVAL时才会返回 */
    short revents;    /* 是一个内核设置的输出值输出值，它的值是events设置的任何值，这是代表事件发生，或是POLLHUP, POLLERR, POLLNVAL等代表错误发生的事件 */
};
```
  > 可监听的事件events如下：
  > - POLLIN: 描述符有数据可读
  > - POLLPRI: 文件描述符发生了异常
  > - POLLOUT: 描述符可写
  > - POLLRDHUP: 对方TCP套接字关闭了连接，需要添加定义`_GNU_SOURCE`
  > - POLLERR: 只在revents中出现，表示poll过程发生了错误
  > - POLLHUP: 只在revents中出现，表示管道或套接字的连接被对方关闭，只有将缓冲区的数据都读出后，才会返回0
  > - POLLNVAL: 只在revets中出现，表示操作失败，描述符没有打开

- `int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *tmo_p, const sigset_t *sigmask);`
  > `#include <signal.h>`
  >
  > `#include <poll.h>`
  >
  > 描述：和poll函数的功能类似，只是超时时间精度不同，以及ppoll修改了poll阻塞时的信号屏蔽
  >
  > fds: 文件描述和文件操作的集合
  >
  > nfds: 指定文件描述符的个数
  >
  > tmo_p: 指定poll的超时时间
  >
  > sigmask: 指定信号掩码， 当该值为NULL时，此函数和poll只有时间参数的区别
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/poll.2.html)
  >
  > 返回：成功-返回正数，代表发生事件的描述符数量，返回0代表等待时间超时，文件描述符没有事件发生；失败-返回-1，并设置errno
```C
struct timespec {
    long    tv_sec;         /* 秒 */
    long    tv_nsec;        /* 纳秒 */
};

// 下面的执行
ready = ppoll(&fds, nfds, tmo_p, &sigmask);
// 和这里的相似
sigset_t origmask;
int timeout;

timeout = (tmo_p == NULL) ? -1 :
           (tmo_p->tv_sec * 1000 + tmo_p->tv_nsec / 1000000);
pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
ready = poll(&fds, nfds, timeout);
pthread_sigmask(SIG_SETMASK, &origmask, NULL);
```

#### epoll
> [epoll - I/O event notification facility](http://man7.org/linux/man-pages/man7/epoll.7.html)
>
> epoll多路复用方式有效的解决了select和poll机制的问题，epoll的核心概念时epoll实列，一个内核中的数据结构，从用户空间的角度来看，就是两个列表容器：
> - interest列表：用于保存需要进行监控的文件描述列表
> - ready列表：保存了那些已经可以进行IO操作的文件描述符
>
> 使用下面的这些函数来进行epoll实例的管理
> - `int epoll_create(int size);`或`int epoll_create1(int flags);`
> - `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);`
> - `int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);`或`int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask);`
>

> epoll的触发方式
> - Level-triggered：水平触发，只要缓冲区内有没有读完的数据，就会一直触发
> - edge-triggered：边沿触发，只适用于非阻塞IO，只触发一次

- `int epoll_create(int size);`
  > `#include <sys/epoll.h>`
  >
  > 描述：用于创建epoll实例
  >
  > size：指定最多监听描述符的数量，在Linux2.6.8后该参数被忽略，但是必须设置大于0
  >
  > 返回：成功-返回非负的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/epoll_create.2.html)

- `int epoll_create1(int flags);`
  > `#include <sys/epoll.h>`
  >
  > 描述：和epoll_create一样，用于创建epoll实例
  >
  > flag: 设置epoll实例属性，如果为0，则和epoll_create函数一致，另外的选项有EPOLL_CLOEXEC，该选项会给生成的epoll设置FD_CLOEXEC标志位，这样在进程中调用exec函数族的时候就会将该文件关闭，避免文件描述符泄露给exec后的进程
  >
  > 返回：成功-返回非负的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/epoll_create.2.html)

- `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);`
  > `#include <sys/epoll.h>`
  >
  > 描述：用于添加，修改，删除文件的监听事件到epoll实例中
  >
  > epfd: 指定epoll_create创建的epoll描述符，
  >
  > op: 指定操作码，EPOLL_CTL_ADD-添加文件监听，EPOLL_CTL_MOD-修改文件监听，EPOLL_CTL_DEL-删除文件监听
  >
  > fd: 指定文件描述符
  >
  > event: 指定文件描述符的监听信息
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/epoll_ctl.2.html)
  >
  > epool事件集合：
  > - EPOLLIN：关联的fd可以进行读操作
  > - EPOLLOUT：关联的fd可以进行写操作
  > - EPOLLPRI：关联的fd有紧急可读操作
  > - EPOLLERR：关联的fd发生了错误
  > - EPOLLHUP：关联的fd挂起了
  > - EPOLLET：设置关联的fd以ET触发方式工作，默认为LT
  > - EPOLLONESHOT：只监听一次，需要再监听时需要将文件描述符再添加到inteterst列表中
```C
struct epoll_event {
    uint32_t     events;      /* Epoll 事件 */
    epoll_data_t data;        /* 用户的数据变量 */
};
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;
```

- `int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);`
  > `#include <sys/epoll.h>`
  >
  > 描述：epoll阻塞等待监听事件的发生
  >
  > epfd: 指定epoll描述符
  >
  > events: 是内核获取的事件的集合
  >
  > maxevents: 告诉内核events有少个事件，必须大于0且不能大于size
  >
  > timeout: 设置阻塞超时时间，单位为毫秒，如果传入-1，表示一直阻塞的事件发生，传入0表示就算没有事件发生也会立即返回；
  >
  > 返回：成功-返回获取的事件数量；失败-返回-1并设置errno
  >
  > man 7 [参考]()

- `int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask);`
  > `#include <sys/epoll.h>`
  >
  > 描述：和epoll功能类似，只是添加了信号掩码
  >
  > sigmask: 指定信号掩码
  >
  > 返回：成功-返回获取的事件数量；失败-返回-1并设置errnoo
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/epoll_wait.2.html)
```C
// epoll_pwait相当于下面的调用
sigset_t origmask;

sigprocmask(SIG_SETMASK，＆sigmask，＆origmask);
ready = epoll_wait(epfd，＆events，maxevents，timeout);
sigprocmask(SIG_SETMASK，＆origmask，NULL);
```

#### 参考列表
- [Linux IO模式及 select、poll、epoll详解](https://segmentfault.com/a/1190000003063859)
- [IO多路复用原理剖析](https://juejin.im/post/59f9c6d66fb9a0450e75713f)
- [IO多路复用之select全面总结(必看篇)](https://www.jb51.net/article/101057.htm)
- [epoll机制:epoll_create、epoll_ctl、epoll_wait、close](https://blog.csdn.net/yusiguyuan/article/details/15027821)
- [Linux epoll 详解](http://blog.lucode.net/linux/epoll-tutorial.html)
- [一个完整的 epoll + socket 的例子](https://www.felix021.com/blog/read.php?1879)

#### 案列
- 多客户端案列
> server.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <netinet/in.h>

#define BUF_SIZE 4096
#define PORT 10000
#define LISTEN_BACK 32
#define POOL_SIZE 64

int lsnsock;
int sock_pool[POOL_SIZE];

static void handle_signal(int signo);
static void usage(char *prog_name);
static void version(char *prog_name);

int main(int argc, char *argv[]){
    int     connsock;
    int     i, ret, recv_data_len, send_data_len;
    struct  sockaddr_in srvaddr;
    int     opt;
    int     port = PORT;
    fd_set  readfds;
    int     maxfds;
    char    buf[BUF_SIZE];

    signal(SIGINT, handle_signal);
    signal(SIGHUP, handle_signal);
    signal(SIGKILL, handle_signal);

    while((opt = getopt(argc, argv, "p:hv")) != -1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                break;
            case 'v':
                version(argv[0]);
                break;
            default:
                usage(argv[0]);
        }
    }
    for(i = 0; i < POOL_SIZE; i++){
        sock_pool[i] = -1;
    }

    lsnsock = socket(AF_INET, SOCK_STREAM, 0);
    if(lsnsock == -1){
        perror("create sock error");
        exit(EXIT_FAILURE);
    }
    memset(&srvaddr, 0, sizeof(struct sockaddr_in));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(port);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(lsnsock, (struct sockaddr*)&srvaddr, sizeof(struct sockaddr_in));
    if(ret == -1){
        perror("bind error");
        close(lsnsock);
        exit(EXIT_FAILURE);
    }
    ret = listen(lsnsock, LISTEN_BACK);
    if(ret == -1){
        perror("listen error");
        close(lsnsock);
        exit(EXIT_FAILURE);
    }

    while(1){
        FD_ZERO(&readfds);
        FD_SET(lsnsock, &readfds);
        maxfds = lsnsock;
        for(i = 0; i < POOL_SIZE; i++){
            if(sock_pool[i] != -1){
                FD_SET(sock_pool[i], &readfds);
                maxfds = maxfds > sock_pool[i] ? maxfds : sock_pool[i];
            }
        }
        ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
        if(ret == -1){
            perror("select error");
            continue;
        }
        if(FD_ISSET(lsnsock, &readfds)){
            connsock = accept(lsnsock, NULL, NULL);
            for(i = 0; i < POOL_SIZE; i++){
                if(sock_pool[i] == -1){
                    printf("sock_pool[%d]= %d\n", i, connsock);
                    sock_pool[i] = connsock;
                    break;
                }
            }
        }else{
            for(i = 0; i < POOL_SIZE; i++){
                if(sock_pool[i] != -1){
                    if(FD_ISSET(sock_pool[i], &readfds)){
                        recv_data_len = recv(sock_pool[i], buf, BUF_SIZE, 0);
                        if(recv_data_len == 0){
                            close(sock_pool[i]);
                            sock_pool[i] = -1;
                        }else if(recv_data_len == -1){
                            perror("recv error for socke");
                            continue;
                        }else{
                            printf("from sock[%d]\n", sock_pool[i]);
                            printf("mesg: %s\n", buf);
                            send_data_len = send(sock_pool[i], buf, strlen(buf) + 1, 0);
                            if(send_data_len == -1){
                                perror("send error");
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
}

static void handle_signal(int signo){
    if(signo == SIGINT){
        fprintf(stderr, "received signal SIGINT(%d)\n", signo);
    }else if(signo == SIGHUP){
        fprintf(stderr, "received signal SIGHUP(%d)\n", signo);
    }else if(signo == SIGKILL){
        fprintf(stderr, "received signal SIGKILL(%d)\n", signo);
    }

    close(lsnsock);
    exit(EXIT_SUCCESS);
}

static void usage(char *prog_name){
    printf("Usage: %s options\n", prog_name);
    printf("options:\n");
    printf("-p      port number\n");
    printf("-h      help info\n");
    printf("-v      version info\n");

    exit(EXIT_SUCCESS);
}

static void version(char *prog_name){
    printf("%s:\n", prog_name);
    printf("version-1.0.00\n");

    exit(EXIT_SUCCESS);
}
```
> client.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096

int connsock;

static void usage(char *prog_name);
static void version(char *prog_name);
static void handle_signal(int signo);

int main(int argc, char *argv[]){
    int     i, ret, port;
    int     opt;
    int     send_data_len, recv_data_len;
    char    buf[BUF_SIZE];
    char    *ip_addr;
    struct  sockaddr_in srvaddr;

    signal(SIGINT, handle_signal);
    while((opt = getopt(argc, argv, "p:s:hv")) != -1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                ip_addr = optarg;
                break;
            case 'h':
                usage(argv[0]);
                break;
            case 'v':
                version(argv[0]);
                break;
            default:
                usage(argv[0]);
        }
    }
    connsock = socket(AF_INET, SOCK_STREAM, 0);
    if(connsock == -1){
        perror("create sock error");
        exit(EXIT_FAILURE);
    }
    memset(&srvaddr, 0, sizeof(struct sockaddr_in));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(port);
    srvaddr.sin_addr.s_addr = inet_addr(ip_addr);
    ret = connect(connsock, (struct sockaddr*)&srvaddr, sizeof(struct sockaddr_in));
    if(ret == -1){
        perror("connect error");
        close(connsock);
        exit(EXIT_FAILURE);
    }

    printf("%% ");
    while(fgets(buf, BUF_SIZE, stdin) != NULL){
        if(buf[strlen(buf) - 1] == '\n'){
            buf[strlen(buf) - 1] = 0;
        }
        if(strcmp(buf, "quit") == 0){
            close(connsock);
            exit(EXIT_SUCCESS);
        }
        send_data_len = send(connsock, buf, strlen(buf) + 1, 0);
        if(send_data_len == -1){
            perror("send error");
            continue;
        }
        recv_data_len = recv(connsock, buf, BUF_SIZE, 0);
        if(recv_data_len == -1){
            perror("recv error");
            continue;
        }
        printf("server return: %s\n", buf);
        printf("%% ");
    }
}

static void usage(char *prog_name){
    printf("Usage: %s options\n", prog_name);
    printf("options:\n");
    printf("-h      help info\n");
    printf("-v      version info\n");
    printf("-p      server port\n");
    printf("-s      server ip address\n");

    exit(EXIT_SUCCESS);
}

static void version(char *prog_name){
    printf("%s:\n", prog_name);
    printf("version-1.0.0.00\n");

    exit(EXIT_SUCCESS);
}

static void handle_signal(int signo){
    if(signo == SIGINT){
        fprintf(stderr, "received signal SIGINT(%d)\n", signo);
    }

    close(connsock);
    exit(EXIT_SUCCESS);
}
```
-  epoll_daemon.c
> server.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096
#define PORT 9000
#define CONNECT_POOL 64
#define LISTEN_BACK 128
#define EPOLL_SIZE 1024
#define EPOLL_WAIT_SIZE 128
#define EPOLL_TIME_OUT 60000

struct conn_sock_info {
    int     conn_fd;
    struct  sockaddr_in client_addr;
    int     addr_len;
};

static void usage(const char *prog_name);
static void version(const char *prog_name);

int main(int argc, char *argv[]){
    int     opt, i, j, ret;
    int     port = PORT;
    int     lsn_sock, conn_sock;
    int     epoll_id;
    int     addr_len, epoll_wait_len;
    struct  sockaddr_in server_addr;
    struct  sockaddr_in client_addr;
    struct  epoll_event fd_event, events[EPOLL_WAIT_SIZE];
    struct  conn_sock_info conn_pool[CONNECT_POOL];
    char    buf[BUF_SIZE];

    for(i = 0; i < CONNECT_POOL; i++){
        conn_pool[i].conn_fd= -1;
        memset(&conn_pool[i].client_addr, 0, sizeof(struct sockaddr_in));
        conn_pool[i].addr_len = 0;
    }
    while((opt = getopt(argc, argv, "vhp:")) != -1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                break;
            case 'v':
                version(argv[0]);
                break;
            default:
                usage(argv[0]);
        }
    }

    lsn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(lsn_sock == -1){
        perror("create socket error");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    ret = bind(lsn_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    if(ret == -1){
        perror("bind error");
        close(lsn_sock);
        exit(EXIT_FAILURE);
    }
    ret = listen(lsn_sock, LISTEN_BACK);
    if(ret == -1){
        perror("listen error");
        close(lsn_sock);
        exit(EXIT_FAILURE);
    }

    epoll_id = epoll_create1(EPOLL_CLOEXEC);
    if(epoll_id == -1){
        perror("epoll_create error");
        close(lsn_sock);
        exit(EXIT_FAILURE);
    }
    fd_event.events = EPOLLIN;
    fd_event.data.fd = lsn_sock;
    ret = epoll_ctl(epoll_id, EPOLL_CTL_ADD, lsn_sock, &fd_event);
    if(ret == -1){
        perror("epoll_ctl error");
        close(lsn_sock);
        exit(EXIT_FAILURE);
    }

    for(; ;){
        epoll_wait_len = epoll_wait(epoll_id, events, EPOLL_WAIT_SIZE, EPOLL_TIME_OUT);
        if(ret == -1){
            perror("epoll_wait error");
            continue;
        }
        for(i = 0; i < epoll_wait_len; i++){
            if(events[i].events & EPOLLERR || events[i].events & EPOLLHUP){
                fprintf(stderr, "EPOLLERR or EPOLLHUP");
                continue;
            }
            if(events[i].data.fd == lsn_sock){
                conn_sock = accept(lsn_sock, (struct sockaddr*)&client_addr, &addr_len);
                if(conn_sock == -1){
                    perror("accept error");
                    continue;
                }
                printf("connected from %s[%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                for(j = 0; j < CONNECT_POOL; j++){
                    if(conn_pool[j].conn_fd == -1){
                        conn_pool[j].conn_fd = conn_sock;
                        conn_pool[j].client_addr = client_addr;
                        conn_pool[j].addr_len = addr_len;

                        break;
                    }
                }

                fd_event.events = EPOLLIN;
                fd_event.data.fd = conn_sock;
                ret = epoll_ctl(epoll_id, EPOLL_CTL_ADD, conn_sock, &fd_event);
                if(ret == -1){
                    perror("epoll_ctl error for conn_sock");
                    continue;
                }
            }else{
                for(j = 0; j < CONNECT_POOL; j++){
                    if(conn_pool[j].conn_fd == -1){
                        continue;
                    }else{
                        if((events[i].events & EPOLLIN) && (events[i].data.fd == conn_pool[j].conn_fd)){
                            ret = recv(conn_pool[j].conn_fd, buf, BUF_SIZE, 0);
                            if(ret == -1){
                                perror("recv error");
                                break;
                            }else if(ret == 0){
                                fd_event.events = EPOLLIN;
                                fd_event.data.fd = conn_pool[j].conn_fd;
                                ret = epoll_ctl(epoll_id, EPOLL_CTL_DEL, conn_pool[j].conn_fd, &fd_event);
                                if(ret == -1){
                                    perror("epoll_ctl error for EPOLL_CTL_DEL");
                                }

                                close(conn_pool[j].conn_fd);

                                conn_pool[j].conn_fd = -1;
                                memset(&conn_pool[j].client_addr, 0, sizeof(struct sockaddr_in));
                                conn_pool[j].addr_len = 0;

                            }else{
                                printf("received from %s[%d] : %s\n",
                                    inet_ntoa(conn_pool[j].client_addr.sin_addr),
                                    ntohs(conn_pool[j].client_addr.sin_port),
                                    buf);
                                ret = send(conn_pool[j].conn_fd, buf, strlen(buf) + 1, 0);
                                if(ret == -1){
                                    perror("send error");
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

static void usage(const char *prog_name){
    printf("Usage: %s options\n", prog_name);
    printf("options:\n");
    printf("-h      help info\n");
    printf("-v      version info\n");
    printf("-p      port number\n");

    exit(EXIT_SUCCESS);
}

static void version(const char *prog_name){
    printf("%s\n", prog_name);
    printf("version.1.0.0.1");

    exit(EXIT_SUCCESS);
}
```

> client.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF_SIZE 4096

int conn_sock;

static void usage(const char *prog_name);
static void version(const char *prog_name);
static void sig_handler(int signo);

int main(int argc, char *argv[]){
    int     opt, ret;
    int     port;
    char    *server;
    char    buf[BUF_SIZE];
    struct  sockaddr_in server_addr;
    struct  sigaction sigac;

    sigac.sa_handler = sig_handler;
    sigemptyset(&sigac.sa_mask);
    sigac.sa_flags = 0;
    ret = sigaction(SIGINT, &sigac, NULL);
    if(ret == -1){
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }
    if(argc < 2){
        fprintf(stderr, "options error\n");
        usage(argv[0]);
    }
    while((opt = getopt(argc, argv, "vhp:s:")) != -1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                server = optarg;
                break;
            case 'h':
                usage(argv[0]);
                break;
            case 'v':
                version(argv[0]);
                break;
            default:
                usage(argv[0]);
        }
    }
    if(port == 0){
        fprintf(stderr, "port error\n");
        usage(argv[0]);
    }
    if(server == NULL){
        fprintf(stderr, "server ip error\n");
        usage(argv[0]);
    }

    conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_sock == -1){
        perror("create socket error");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server);
    server_addr.sin_port = htons(port);
    ret = connect(conn_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if(ret == -1){
        perror("connect error");
        close(conn_sock);
        exit(EXIT_FAILURE);
    }
    printf("%% ");
    while(fgets(buf, BUF_SIZE, stdin) != NULL){
        if(buf[strlen(buf) - 1] == '\n'){
            buf[strlen(buf) - 1] = 0;
        }
        ret = send(conn_sock, buf, strlen(buf) + 1, 0);
        if(ret == -1){
            perror("send error");
            close(conn_sock);
            exit(EXIT_FAILURE);
        }
        ret = recv(conn_sock, buf, BUF_SIZE, 0);
        if(ret == -1){
            perror("recv error");
            close(conn_sock);
            exit(EXIT_FAILURE);
        }
        printf("recv: %s\n", buf);
        printf("%% ");
    }
}

static void usage(const char *prog_name){
    printf("Usage: %s options\n", prog_name);
    printf("options:\n");
    printf("-p      port number\n");
    printf("-s      server ip address\n");
    printf("-h      help info\n");
    printf("-v      version info\n");

    exit(EXIT_SUCCESS);
}

static void version(const char *prog_name){
    printf("%s\n", prog_name);
    printf("version 1.0.00\n");

    exit(EXIT_SUCCESS);
}

static void sig_handler(int signo){
    if(signo == SIGINT){
        printf("received signal SIGINT(%d)\n", SIGINT);
    }
    close(conn_sock);
    exit(EXIT_SUCCESS);
}
```