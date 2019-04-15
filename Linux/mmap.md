# MMAP
> mmap是一种内存映射文件的方法，即将一个文件或者其他对象映射到进程的地址空间(一般被映射到堆栈之间的空余部分)，实现文件磁盘地址和进程虚拟地址空间中一段地址的一一映射关系，进程可以使用指针方式读写操作这一段内存，而系统会自动回写脏页到的对应的文件上，即完成对文件的操作不必调用read,write等系统调用，同时，内核空间对这段区域的修改也会直接反映到用户空间，从而可以实现不同进程间的文件共享； 

> 映射建立后，即使关闭文件，映射依然存在，因为映射的是磁盘地址，不是文件本身，和文件句柄没有关系，同时可用于进程间通信的有效地址空间不完全受限于被映射的文件的大小，因为是按页映射。


#### 函数列表
- `void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);`
  > `include <sys/mman.h>`
  >
  > 描述：将指定文件描述符代表的文件映射到进程的虚拟地址空间
  >
  > addr: 指定映射的地址；如果为NULL，则内核会自动选择页面对齐的地址来创建映射，这也是常用的方式；如果不为NULL，则会使用addr作为映射地址的参考，Linux上会在该地址附近寻找页的整数倍地址进行隐射(注意这个地址还需要大于/proc/sys/vm/mmap_min_addr)；如果选择的这个地址已经被映射，则内核会重新选择一个新的映射地址；
  >
  > length: 指定要映射的内存区域大小
  >
  > prot: 内存的保护标志
  >> - PROT_EXEC: 页数据可以被执行
  >> - PROT_READ: 页数据可以读取
  >> - PROT_WRITE: 页可以被写入数据
  >> - PROT_NONE: 页不可被访问，通常不用
  >
  > flags: 指定映射对象的类型
  >> - MAP_FIXED
  >> - MAP_SHARED
  >> - MAP_PRIVATE
  >> - MAP_NORESERVE
  >> - MAP_LOCKED
  >> - MAP_GROWDOWN
  >> - MAP_ANONYMOUS
  >> - MAP_32BIT
  >> - MAP_POPULATE
  >> - MAP_NONBLOCK
  >
  > fd: 需要映射的文件描述符
  >
  > offset: 即文件从哪里开始映射，通常是0，代表从文件开始映射，该值大小应为PAGE_SIZE的整数倍
  >
  > 返回：成功-返回指向映射区域的指针。错误-返回MAP_FAILED
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/mmap.2.html)

![mmap映射关系](https://upload-images.jianshu.io/upload_images/1723697-d584d0838eab653b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/678/format/webp)

- `int munmap(void *addr, size_t length);`
  > `#include <sys/mman.h>`
  >
  > 描述：取消内存的映射
  >
  > addr: mmap函数返回地址，指定映射区域的开始地址
  >
  > length: 映射区域的长度
  >
  > 返回：成功-返回0；失败-返回-1，并设置errno

- `int msync(void *addr, size_t length, int flags);`
  > `#include <sys/mman.h>`
  >
  > 描述：进程对映射空间的内容修改并不会立刻写回到磁盘中，往往需要在调用munmap()后才执行该操作。该函数可以将映射空间中修改的内容同步回底部文件系统；
  >
  > addr: mmap返回的地址，指定映射区的开始地址
  >
  > length: 指定映射区的字节长度
  >
  > flags: 同步属性，确保指定MS_ASYNC或MS_SYNC其中一个
  >> - MS_ASYNC: 异步方式，会将数据放入调度队列，并不会等待数据的写入磁盘
  >> - MS_SYNC: 同步方式，将数据写入到磁盘后返回
  >> - MS_INVALIDATE: 同步完成后，使得文件的其他映射失效，从而使得共享该文件的其他进程去重新获取最新值
  >
  > 返回：成功-返回0；错误-返回-1，并设置errno
  >
  > man 7 [参考](http://man7.org/linux/man-pages/man2/msync.2.html)

#### 参考列表
- [认真分析mmap：是什么 为什么 怎么用](https://www.cnblogs.com/huxiao-tee/p/4660352.html)
- [mmap, munmap - map or unmap files or devices into memory](http://man7.org/linux/man-pages/man2/mmap.2.html)
- [mmap x86小于0x10000的虚地址](https://richardustc.github.io/2013-05-21-2013-05-21-min-mmap-addr.html)
- [Linux 内存映射函数 mmap（）函数详解](https://blog.csdn.net/DLUTBruceZhang/article/details/9080173)
- [mlock家族：锁定物理内存](https://www.cnblogs.com/lixiaofei1987/p/3205002.html)
- [mmap详解](https://nieyong.github.io/wiki_cpu/mmap%E8%AF%A6%E8%A7%A3.html)
- [共享内存之——mmap内存映射](https://www.jianshu.com/p/096e1b58c678)

#### 案列
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define handle_error(msg)\
        do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]){
        char    *addr;
        int     fd;
        struct  stat sb;
        off_t   offset, pa_offset;
        size_t  length;
        ssize_t s;

        if(argc < 3 || argc > 4){
                fprintf(stderr, "%s file offset [length]\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        fd = open(argv[1], O_RDONLY);
        if(fd == -1){
                handle_error("open error");
        }
        if(fstat(fd, &sb) == -1){
                handle_error("fstat error");
        }

        offset = atoi(argv[2]);
        // ???
        pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
        // ???
        if(offset >= sb.st_size){
                fprintf(stderr, "offset is past end of file\n");
                close(fd);
                exit(EXIT_FAILURE);
        }

        if(argc == 4){
                length = atoi(argv[3]);
                if(offset + length > sb.st_size){
                        length = sb.st_size - offset;
                }
        }else{
                length = sb.st_size - offset;
        }

        addr = mmap(NULL, length + offset - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
        if(addr == MAP_FAILED){
                handle_error("mmap error");
        }

        s = write(STDOUT_FILENO, addr + offset -pa_offset, length);
        if(s != length){
                if(s == -1){
                        handle_error("write errror");
                }
                fprintf(stderr, "partial write");
                close(fd);
                exit(EXIT_FAILURE);
        }

        munmap(addr, length + offset - pa_offset);
        close(fd);

        exit(EXIT_SUCCESS);
}
```
