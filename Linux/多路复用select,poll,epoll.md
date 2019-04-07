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
  > ndfs：被设置为三个文件集合中的最大描述符 + 1；这样就能保证每个文件描述符都被检查到了，被检测的文件描述符从0, 1, 2, ..., nfds - 1;
  >
  > readfds: 用于

#### 参考列表
- [Linux IO模式及 select、poll、epoll详解](https://segmentfault.com/a/1190000003063859)
- [IO多路复用原理剖析](https://juejin.im/post/59f9c6d66fb9a0450e75713f)
- [Linux socket网络编程之聊天室（三）：select异步通讯实现](https://blog.csdn.net/YEYUANGEN/article/details/6742109)
