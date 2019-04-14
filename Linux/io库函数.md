# Linux IO
#### 1. Linux IO模型
> - 阻塞IO(blocking I/O-BIO)
> - 非阻塞IO(noblocking I/O-NIO)
> - 多路复用IO(multplexing I/O)
> - 信号驱动IO(signal-driven I/O)
> - 异步IO(asynchronous I/O-AIO)

> **标准IO库提供了缓冲功能**
> - 全缓冲：即填满IO缓冲区后才进行IO操作
> - 行缓冲：遇到换行符执行IO操作
> - \*   : 无缓冲

#### 2. 非缓冲IO
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
  > 描述：为oldfd描述符生成新的描述符，是当前可用文件描述符的最小值，这个文件描述符指向oldfd所拥有的文件表项，复制成功后，新旧描述符都可以使用，并且它们共享文件偏移和文件标志位，但是两个文件描述符有独立的文件描述符位，如FD_CLOEXEC；
  >
  > oldfd: 指定旧文件描述符
  >
  > 返回：成功-返回新的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [返回](http://man7.org/linux/man-pages/man2/dup.2.html)

- `int dup2(int oldfd, int newfd);`
  > `#include <unistd.h>`
  >
  > 描述：和dup功能类似，但不是返回最小的文件描述符，而是使用参数提供的newfd；如果newfd已经打开，则会不做提醒地关闭newfd然后重新使用，这个关闭到重新使用newfd文件描述符的操作是原子性的；如果newfd和oldfd相同，则该函数什么都不做，直接返回newfd;
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
  > 描述：功能和`dup2`相同，只是可以通过flags强制设置新文件描述符的的标志位，如果newfd和oldfd相同，则会报错
  >
  > oldfd: 指定旧的文件描述符
  >
  > newfd: 指定使用的新的文件描述符
  >
  > flags: 设置文件描述符标志位，O_CLOEXE-设置文件描述符的close-on-exec标志位
  >
  > 返回：成功-返回新的文件描述符；失败-返回-1，并设置errno
  >
  > man 7 [返回](http://man7.org/linux/man-pages/man2/dup.2.html)
  >
  > noee: dup3() is Linux-specific.


- `void sync(void);`
  > `#include <unistd.h>`
  >
  > 描述：将文件系统的脏数据刷入磁盘，包括文件元数据和文件内容,通常将所有修改过的块缓冲区排入写队列，然后就返回，它不等待实际写磁盘的操作，系统上update的系统守护进程会周期性地(一般位30s)调用sync函数，就保证了定时冲洗内核的块缓冲区；
  >
  > 返回：总是成功，不会返回失败
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/sync.2.html)

- `int syncfs(int fd);`
  > `#include <unistd.h>`
  >
  > 描述：将指定文件描述符的脏数据刷入磁盘，包括文件元素据和文件内容
  >
  > fd: 指定文件描述符
  >
  > 返回：成功-返回0，失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/sync.2.html)

- `int fsync(int fd);`
  > `#include <unistd.h>`
  >
  > 描述：该函数会将文件描述符的数据和元数据刷入磁盘，数据库应用的原子操作通常使用该函数，但是它会进行两次IO，一次刷入数据，一次刷入元数据，这会使得写入性能下降；会等待磁盘操作结束后再返回；v
  >
  > fd: 指定文件描述符
  >
  > 返回：成功-返回0；失败-返回-1并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/fdatasync.2.html)

- `int fdatasync(int fd);`
  > `#include <unistd.h>`
  >
  > 描述：只同步文件描述符的数据，不同步元数据
  >
  > fd: 指定文件描述符
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno

#### 3. 标准IO
###### 3.1 打开关闭
- `FILE *fopen(const char *pathname, const char *mode);`
  > `#include <stdio.h>`
  >
  > 描述：打开一个文件并并关联一个流
  >
  > pathnam: 文件名名称
  >
  > mode: 打开文件的模式
  >
  > 返回：成功-返回文件流指针；失败-返回NULL，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fopen.3.html)

| fopen() mode | open() flags |
| --- | --- |
| r | O_RDONLY |
| r+ | O_RDWR |
| w | O_WRONLY \| O_CREAT \| O_TRUNC |
| w+ | O_RDWR \| O_CREAT \| O_TRUNC |
| a | O_WRONLY \| O_CREAT \| O_APPEND |
| a+ | O_RDWR \| O_CREAT \| O_APPEND |

> 如果为二进制文件，相应的加上b标志位

- `FILE *fdopen(int fd, const char *mode);`
  > `#include <unistd.h>`
  >
  > 描述：将文件描述符转换为文件指针，注意mode需要和文件描述符打开的方式一致
  >
  > fd: 指定文件描述符
  >
  > mode: 指定文件打开模式
  >
  > 返回：成功-返回文件流指针；失败-返回NULL，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fopen.3.html)

- `FILE *freopen(const char *pathname, const char *mode, FILE *stream);`
  > `#include <unistd.h>`
  >
  >  描述：文件重定向流，一般用于标准输入输出的重定向，如果stream已经存在，则会将其关闭
  >
  > pathname: 指定目录文件
  >
  > mode: 指定文件打开模式
  >
  > stream: 指定重定向的流，一般为stdin,stdout,stderr
  >
  > 返回：成功-返回文件流指针；失败-返回NULL，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fopen.3.html)

- `int fclose(FILE *stream);`
  > `#include <unistd.h>`
  >
  > 描述：关闭文件流，注意使用fclose文件的时候只会将用户空间的缓存刷入内核空间缓存，不会保证数据写入磁盘
  >
  > stream: 指定关闭的文件流
  >
  > 返回：成功-返回0；失败-返回EOF，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fclose.3.html)

###### 3.2 字符IO
- `int fgetc(FILE *stream)`
  > `#include <stdio.h>`
  >
  > 描述：从文件流获取一个字符
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回读取到的字符；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fgetc.3.html)

- `int getc(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：和fgetc的功能一致，从文件流中获取一个字符
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回读到的字符；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fgetc.3.html)

- `int getchar(void);`
  > `#include <stdio.h>`
  >
  > 描述：功能和getc一致，从标准输入获取一个字符
  >
  > 返回：成功-返回读到的字符；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fgetc.3.html)

- `int ungetc(int c, FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：将字符写入文件流中
  >
  > c: 指定的字符
  >
  > stream：指定的文件流
  >
  > 返回：成功-返回写入的字符；失败-返回EOF；
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fgetc.3.html)

- `int fputc(int c, FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：将字符写入文件流中
  >
  > c: 指定写入字符
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回写入的字符；失败-返回EOF，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fputc.3p.html)

- `int putc(int c, FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：将指定的字符写入到流中
  >
  > c: 指定写入的字符
  >
  > stream：指定写入的流
  >
  > 返回：成功-返回写入的字符；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/putc.3p.html)

- `int putchar(int c);`
  > `#include <stdio.h>`
  >
  > 描述：将字符输出到标准输出
  >
  > c: 指定的字符
  >
  > 返回：成功-返回写入的字符；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/putchar.3p.html)

###### 3.2 字符串IO
- `char *fgets(char *restrict s, int n, FILE *restrict stream);`
  > `#include <stdio.h>`
  >
  > 描述：从文件流中读取字符串，读取返回有三种情况，1-读取到了n-1个字符；2-读取到了换行符；3-读取到了文件末尾，即EOF；该函数会更新流的访问时间属性
  >
  > s: 读取文件缓存
  >
  > n: 最多读取的字节数
  >
  > stream: 指定流
  >
  > 返回：成功-返回指向s的指针；失败-返回NULL，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fgets.3p.html)

- `char *gets(char *s);`
  > `#include <stdio.h>`
  >
  > warging: 不要使用该该函数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/gets.3.html)

- `int fputs(const char *restrict s, FILE *restrict stream);`
  > `#include <stdio.h>`
  >
  > 描述：将缓冲区s中的字符写入文件流中，写入的终点为空字符，并且空字符不写入文件流，也不会像puts追加换行符；在该文件执行成功后，系统会标记文件的最后修改时间
  >
  > s: 指定缓冲区
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回非负数字；失败-返回EOF，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fputs.3p.html)

- `int puts(const char *s);`
  > `#include  <stdio.h>`
  >
  > 描述：将字符串s添加换行符后输出到标准输出
  >
  > s: 输出的字符
  >
  > 返回：成功-返回非负的整数；失败-返回EOF
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/puts.3.html)

###### 3.3 二进制IO
- `size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：从文件流读取size * nmemb个字节的数据到ptr中，size代表了每个对象的有多少个字节，而nmemb则代表要读取多少个对象
  >
  > ptr: 保存读出数据的缓存
  >
  > size: 指定每个对象的大小，单位为字节
  >
  > nmemb: 指定了需要读取多少个对象
  >
  > stream: 指定流
  >
  > 返回：成功-返回读取对象的个数；失败-需要使用`feof`和`ferror`进行检测查看似乎发生了错误
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fread.3.html)

- `size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：从缓存ptr中写入nmemb个对象到文件流中，每个对象的大小为size字节
  >
  > ptr: 缓存地址
  >
  > size: 指定对象的大小
  >
  > nmemb: 指定写入的对象
  >
  > stream: 指定输出的流
  >
  > 返回：成功-返回写入的对象个数；失败-需要使用`feof`和`ferror`进行检测查看似乎发生

###### 3.4 文件错误检测
- `int feof(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：检查流是否读到了文件流末尾，有时候在使用标准IO读取文件时，返回EOF有可能发生了错误，所以可以使用该函数检测是否读到了文件末尾
  >
  > stream: 指定文件流
  >
  > 返回：设置时返回非0值，否则返回0
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/feof.3.html)

- `int ferror(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：检测文件流是否发生了错误，如果发生了错误，则返回非0值，否则返回0值
  >
  > stream: 指定文件流
  >
  > 返回：发生错误时返回非0值，否则返回0
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/feof.3.html)

- `int fileno(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：返回文件流的描述符
  >
  > stream: 指定文件描述符
  >
  > 返回：成功-返回文件描述符，失败-返回-1并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/feof.3.html)

- ` void clearerr(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：清除文件流上的EOF标志位和错误标志位
  >
  > stream: 指定文件流
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/feof.3.html)

###### 3.5 定位流
- `long ftell(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：返回文件流的文件指针的值
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回当前文件指针的值；错误-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/ftell.3p.html)

- `off_t ftello(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：功能和ftell类似，只是返回值类型为off_t，也返回文件指针的当前值，并且在成功返回的时候可能也会设置errno
  >
  > stream: 指定文件流
  >
  > 返回：成功-返回当前文件指针的值；错误-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/ftell.3p.html)

- `int fseek(FILE *stream, long offset, int whence);`
  > `#include <stdio.h>`
  >
  > 描述：设置文件流的文件指针
  >
  > stream: 指定文件流
  >
  > offset: 指定相对偏移量，以字节为单位
  >
  > whence：SEEK_SET-偏移量相对于文件头；SEEK_CUR-偏移量先对于当前指针；SEEK_END-偏移量相对于文件末尾；
  >
  > 返回：成功-返回0；失败-返回-1并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fseek.3.html)

- `void rewind(FILE *stream);`
  > `#include <stdio.h>`
  >
  > 描述：将文件指针设置到文件开始
  >
  > stream: 指定文件流
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fseek.3.html)

- `int fgetpos(FILE *stream, fpos_t *pos);`
  > `#include <stdio.h>`
  >
  > 描述：将文件流中文件指针的当前位置(相对与文件开始位置)返回到pos结构体中
  >
  > stream: 指定文件流
  >
  > pos: 文件指针
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fseek.3.html)
```C
typedef struct
{
    __off_t   __pos;  // 双下划线
    __mbstate_t    __state;
}fpos_t;
```

- `int fsetpos(FILE *stream, const fpos_t *pos);`
  > `#include <stdio.h>`
  >
  > 描述：设置文件流中的文件指针
  >
  > stream: 指定文件流
  >
  > pos: 文件指针
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/fseek.3.html)

###### 3.6 格式化IO
- `int printf(const char *format, ...);`
  > `#include <stdio.h>`
  >
  > 描述：将文本格式化输出到标准输出
  >
  > format: 输出格式
  >
  > 返回：成功-返回输出的字符数量；失败-返回负数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/printf.3.html)

- `int fprintf(FILE *stream, const char *format, ...);`
  > `#include <stdio.h>`
  >
  > 描述：将输出格式化内容输出到文件流
  >
  > stream: 指定文件流
  >
  > format: 指定格式化部分
  >
  > 返回：成功-返回输出的字符数量；失败-返回负数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/printf.3.html)

- `int dprintf(int fd, const char *format, ...);`
  > `#include <stdio.h>`
  >
  > 描述：功能和frpintf一致，只是将格式化内容输出到了文件描述符中，而不是文件流
  >
  > fd: 指定文件描述符
  >
  > format: 指定格式化内容
  >
  > 返回：成功-返回输出的字符数量；失败-返回负数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/printf.3.html)

- `int sprintf(char *str, const char *format, ...);`
  > `#include <stdio.h>`
  >
  > 描述：将格式化内容输出到str指定缓冲区中，注意缓冲区的大小是否足够
  >
  > str: 指定缓冲区地址
  > 
  > format: 指定格式化内容
  >
  > 返回：成功-返回输出的字符数量；失败-返回负数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/printf.3.html)

- `int snprintf(char *str, size_t size, const char *format, ...);`
  > `#include <stdio.h>`
  >
  > 描述：将最多size和字节的格式化内容输出到str指定缓冲区中，size的值包括format的空白符，如果字符串的长度多余size，则会截断，并在字符串后添加空白符，返回-1
  >
  > str: 指定缓冲区的地址
  >
  > size: 指定最多输入的字节数量
  >
  > format: 格式化内容
  >
  > 返回：成功-返回输出的字符数量；失败-返回负数
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man3/printf.3.html)

- `int vprintf(const char *format, va_list ap);`
- `int vfprintf(FILE *stream, const char *format, va_list ap);`
- `int vdprintf(int fd, const char *format, va_list ap);`
- `int vsprintf(char *str, const char *format, va_list ap);`
- `int vsnprintf(char *str, size_t size, const char *format, va_list ap);`
  > `#include <stdarg>`
  >
  > 描述：功能和标准格式化类似，就是参数有区别；

#### 参考列表
- [高并发I/O模型](http://lpbobo.com/2016/04/21/%E9%AB%98%E5%B9%B6%E5%8F%91IO%E6%A8%A1%E5%9E%8B/)
- [一步步理解Linux IO（1）–Linux的IO和标准IO库](https://836811384.iteye.com/blog/1978921)
- [What is the difference between fsync and syncfs?](https://stackoverflow.com/questions/48171855/what-is-the-difference-between-fsync-and-syncfs)
- [feof() - C庫函數](http://tw.gitbook.net/c_standard_library/c_function_feof.html)


#### 案列
- read_device.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_SIZE 4096

static void usage(const char *prog_name);
static void version(const char *prog_name);

int main(int argc, char *argv[]){
    int     opt, ret;
    int     size = DEFAULT_SIZE;
    int     readfd;
    char    *device;
    char    *buf;

    if(argc < 2){
        fprintf(stderr, "options error\n");
        usage(argv[0]);
    }
    while((opt = getopt(argc, argv, "vhd:s:")) != -1){
        switch(opt){
            case 'd':
                device = optarg;
                break;
            case 's':
                size = atoi(optarg);
                break;
            case 'v':
                version(argv[0]);
                break;
            case 'h':
                usage(argv[0]);
                break;
            default:
                fprintf(stderr, "error option\n");
                usage(argv[0]);
        }
    }
    buf = (char *)malloc(size + 1);
    if(buf == NULL){
        fprintf(stderr, "malloc errror\n");
        exit(EXIT_FAILURE);
    }

    readfd = open(device, O_RDONLY);
    if(readfd == -1){
        perror("open error");
        free(buf);
        exit(EXIT_FAILURE);
    }
    ret = read(readfd, buf, size);
    if(ret == -1){
        perror("read error");
        free(buf);
        exit(EXIT_FAILURE);
    }
    buf[ret + 1] = 0;
    printf("%s\n", buf);

    free(buf);
    close(readfd);
    exit(EXIT_SUCCESS);
}

static void usage(const char *prog_name){
    printf("Usage: %s options\n", prog_name);
    printf("options:\n");
    printf("-s      read size in byte\n");
    printf("-d      device\n");
    printf("h       help info\n");
    printf("v       version info\n");

    exit(EXIT_SUCCESS);
}

static void version(const char *prog_name){
    printf("%s\n", prog_name);
    printf("Version 1.0.00\n");

    exit(EXIT_FAILURE);
}
```

- freopen_daemon.c
```C
#include <stdio.h>

int main(int argc, char *argv[]){
        if(freopen("test.stdout", "w", stdout) == NULL){
                fprintf(stderr, "freopen error\n");

                return 1;
        }
        printf("freopen successlly\n");

        fclose(stdout);
        return 0;
}
```

- fputc_daemon.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
        time_t  now;
        int     ret;

        time(&now);
        printf("the time is ");
        ret = fputs(asctime(localtime(&now)), stdout);
        if(ret == EOF){
                perror("fputs error");
                exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
}
```
