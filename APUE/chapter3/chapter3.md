# 文件I/O
> 对于内核而言，所有打开的文件都通过文件描述符引用，文件描述符是一个非负整数，文件描述符的取值范围为0~OPEN_MAX - 1, OPEN_MAX的值可以使用`ulimit -n`查看，默认为1024，也可以使用`getconf OPEN_MAX`；当打开一个现有或创建一个新文件时，内核向进程返回一个文件描述符；

> Unix系统shell把文件描述符0(STDOUT_FILENO)与进程的标准输入关联，文件描述符1(STDIN_FILENO)与标准输出关联，文件描述符2(STDERR_FILENO)与标准错误关联

> 由open和openat函数返回的文件描述符一定是最小的未用描述符数值。

> 在创建或打开文件的时候，如果文件名称大于NAME_MAX或目录最大值PATH_MAX，则会报错；(`getconf NAME_MAX /usr`默认为255，`getconf PATH_MAX /usr`默认为4096)

> 当一个进程终止时，内核会自动关闭它所有的打开文件

> 当打开一个文件时，除非指定O_APPEND选项，否则该偏移量被设置为0

> 文件的偏移量可以大于文件的当前长度，这种情况下，对该文件的下一次写将加长该文件，并在文件中构成一个空洞，这是允许的，位于文件中但没有写过的字节都被读为0，文件中的空洞并不要求在磁盘上占用存储区；

#### 库函数列表
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
  > man 7 [参考](http://man7.org/linux/man-pages/man2/open.2.html)

- `int creat(const char *path, mode_t mode);`
  > `#include <sys/stat.h>`
  >
  > `#include <fcntl.h>`
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/creat.3p.html)

- `int close(int fd);`
  > `#include <unistd.h>`
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/close.2.html)

- `off_t lseek(int fd, off_t offset, int whence);`
  > `#include <unistd.h>`
  >
  > `#include <sys/types.h>`
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/lseek.2.html)
