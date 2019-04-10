# 多路复用select,pool,epool
>I/O多路复用(又被称为“事件驱动”)，它是一种机制，可以监视多个文件描述符，一旦某个文件描述符就绪，就可以通知应用程序进行相应的读写操作；

> **IO多路复用适用如下场合:**
> - 当客户处理多个描述字时（一般是交互式输入和网络套接口），必须使用I/O复用
> - 如果一个TCP服务器既要处理监听套接口，又要处理已连接套接口，一般也要用到I/O复用
> - 如果一个服务器即要处理TCP，又要处理UDP，一般要使用I/O复用
> - 如果一个服务器要处理多个服务或多个协议，一般要使用I/O复用

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


#### 参考列表
- [Linux IO模式及 select、poll、epoll详解](https://segmentfault.com/a/1190000003063859)
- [IO多路复用原理剖析](https://juejin.im/post/59f9c6d66fb9a0450e75713f)
- [IO多路复用之select全面总结(必看篇)](https://www.jb51.net/article/101057.htm)

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