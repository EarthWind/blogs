# Linux IO
#### Linux IO模型
> - 阻塞IO(blocking I/O-BIO)
> - 非阻塞IO(noblocking I/O-NIO)
> - 多路复用IO(multplexing I/O)
> - 信号驱动IO(signal-driven I/O)
> - 异步IO(asynchronous I/O-AIO)

#### 非缓冲IO
- `int open(const char *pathname, int flags);`
- `int open(const char *pathname, int flags, mode_t mode);`
- `int openat(int dirfd, const char *pathname, int flags);`
- `int openat(int dirfd, const char *pathname, int flags, mode_t mode);`
  > `#include <sys/types.h>`
  >
  > `#include <sys/stat.h>`
  >
  > `#include <fcntl.h>`
  >
  > 返回：成功-返回文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/open.2.html)
  
  | flags | 描述|
  | --- | --- |
  | O_RDONLY | |
  | O_WRONLY | |
  | O_RDWR | |
  | O_APPEND | 以追加模式打开文件，在write操作之前，将文件指针指定到文件末尾；修改文件指针和写入数据时一个原子操作 |
  | O_ASYNC | 使能信号驱动IO |
  | O_CLOEXEC | |
  | O_CREAT | |
  | O_DIRECT | |
  | O_DIRECTORY | |
  | O_DSYNC | |
  | O_EXCL | |
  | O_LARGEFILE | |
  | O_NOATIME | |
  | O_NOCTTY | |
  | O_FOLLOW | |
  | O_NONBLOCK | |
  | O_PATH | |
  | O_SYNC | |
  | O_TMPFILE | |
  | O_TRUNC | |

- `int close(int fd);`
  > `#include <unistd.h>`
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/close.2.html)

- `off_t lseek(int fd, off_t offset, int whence);`
  > `#include <unistd.h>`
  >
  > `#include <sys/types.h>`
  >
  > fd: 指定使用open打开的文件描述符
  >
  > offset: 指定文件指针的偏移量，单位为字节
  >
  > whence :
  >> - SEEK_SET: 文件指针相对于为文件开始位置的偏移量
  >> - SEEK_CUR: 文件指针相对于当前位置的偏移量
  >> - SEEK_END: 文件指针相对于文件末尾的偏移量
  >
  > 返回：成功-返回距离文件开始处的偏移量；失败-返回-1(off_t -1)，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/lseek.2.html)
  >
  > note: 在文件末尾使用正的偏移量时允许的，这时会产生文件"空洞"，其内容在读出时为'\0'空字符，并且在磁盘上不占据空间，除非被覆盖写掉为止；

- `ssize_t read(int fd, void *buf, size_t count);`
  > `#include <unistd.h>`
  >
  > fd: 指定文件描述符
  >
  > buf: 保存读取缓冲区
  >
  > count: 最多读取的字节数，count的最大值为SSIZE_MAX(32位和64位系统都是0x7ffff000 (2,147,479,552))字节
  >
  > 返回：成功-返回读取的字节数量；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/read.2.html)

- `ssize_t write(int fd, const void *buf, size_t count);`
  > `#include <unistd.h>`
  >
  > 描述：将缓冲区最多count字节数据写入文件描述符中
  >
  > fd: 指定文件描述符
  >
  > buf: 指定缓冲区
  >
  > count: 指定写入的最大字节数，count的最大值为SSIZE_MAX(32位和64位系统都是0x7ffff000 (2,147,479,552))字节
  >
  > 返回：成功-返回写入的数据字节数，小于或等于count;失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/write.2.html)

- `int dup(int oldfd);`
  > `#include <unistd.h>`
  >
  > 描述：为oldfd描述符生成新的描述符，是当前可用文件描述符的最小值，这个文件描述符指向oldfd所拥有的文件表项
  >
  > oldfd: 指定旧文件描述符
  >
  > 返回：成功-返回新的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [返回](http://man7.org/linux/man-pages/man2/dup.2.html)

- `int dup2(int oldfd, int newfd);`
  > `#include <unistd.h>`
  >
  > 描述：和dup功能类似，但不是返回最小的文件描述符，而是使用参数提供的newfd；如果newfd已经打开，则会关闭newfd然后重新使用，这个关闭到重新使用的操作是原子性的；如果newfd和oldfd相同，则该函数什么都不做，直接返回newfd;
  >
  > olfd: 指定旧的文件描述符
  >
  > newfd: 指定新的文件描述符
  >
  > 返回：成功-返回新的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [返回](http://man7.org/linux/man-pages/man2/dup.2.html)

- `int dup3(int oldfd, int newfd, int flags);`
  > `#include <fcntl.h>`
  >
  > `#include <unistd.h>`
  >
  > 
  >
  > 返回：成功-返回新的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [返回](http://man7.org/linux/man-pages/man2/dup.2.html)




#### 参考列表
- [高并发I/O模型](http://lpbobo.com/2016/04/21/%E9%AB%98%E5%B9%B6%E5%8F%91IO%E6%A8%A1%E5%9E%8B/)

