# string.h
## `void *memchr(const void *str, int c, size_t n)`
> 在字符串str的前n个字节搜索第一次出现字符c的位置，返回的指针位置包括字符本身，如果字符没有出现，则返回NULL
```C
const char *str = "hello world";
char ch = 'w';
char *p = NULL;

p = (char*)memchr(str, ch, strlen(str));
printf("p: %s", p);

# 结果：
# p: world
```

## `int memcmp(const void *str1, const void *str2, size_t n)`
> 比较指针str1和指针str2开始的前n个字节，可以进行任何类型的比较，而strcmp则比较的是字符
> - 返回值 < 0: 表示str1的前n个字节小于str2的前n个字节
> - 放回值 = 0：表示str1的前n个字节和str2的前n个字节相等
> - 返回值 > 0：大于
```C
char    *str1 = "hello world";
char    *str2 = "hello ";
int     ret;

ret = memcmp(str1, str2, 6);

# 结果
# ret: 0
```

## `void *memcpy(void *str1, const void *str2, size_t n)`
> 将str2指向的位置的n个字节复制到str1的指针位置，注意和字符串没有关系，所以字符串的结尾符不会管
```C
char    src[] = "hello";
char    dest[6];

memcpy(dest, src, 3);
dest[3] = 0;

# 结果
# dest = "hel"
```

## `void *memmove(void *str1, const void *str2, size_t n)`
> memmove也提供了内存复制的功能，但是和memcpy有区别，memmove是更安全的复制方法，如果目标区域和源区域有重叠的话，memmove能供保证源串在被覆盖之前将重叠区域的内容拷贝的目的区域，复制后源区域的内容会被更改。如果没有重叠部分，则和memcpy功能一致。
```C
char    str1[10] = {"hellowe"};
char    *str2 = str1 + 2;

memmove(str2, str1, 4);

# 结果
# str2: "helle"
```

## `void *memset(void *str, int c, size_t n)`
> memset函数将str地址开始后的n个字节的值设置为整数c对应的字符，一般用来初始化malloc分配的指针
```C
char    str1[] = "helloworld";

memset(str1, '0', 4);

# 结果
# str1: "0000oworld"
```

## `char *strcat(char *dest, const char *src)`
> 将src指向的字符串追加到dest指向的空间，主要dest需要有足够的空间进行追加
```C
char    str1[20] = "hello";
char    str2[] = "world";

strcat(str1, str2);

# 结果
# str1: "helloworld"
```

## `char *strncat(char *dest, const char *src, size_t n)`
> 将src指向的最多n个字节复制到dest

## `char *strchr(const char *str, int c)`
> 返回字符c在str字符串中第一次出现的位置 

## `char *strrchr(const char *str, int c)`
> 返回字符c在字符串str中最后出现的位置，包括字符本身

## `int strcmp(const char *str1, const char *str2)`
> 将字符串str1和str2进行比较

## `int strncmp(const char *str1, const char *str2, size_t n)`
> 比较字符串str1和字符串str2的最多前n个字节

## `int strcoll(const char *str1, const char *str2)`
> 通过设定的环境变量LC_COLLATE，如果该变量位POSIX或C，则和strcmp一样，如果位汉字，则按拼英进行比较，返回字符串的大小关系

## `char *strcpy(char *dest, const char *src)`
> 将字符串src复制到dest指向的位置，dest需要有足够的空间保存src的字符串，并且会自动处理字符串结尾符

## `char *strncpy(char *dest, const char *src, size_t n)`
> 将最多src的n个字节复制到dest

## `size_t strcspn(const char *str1, const char *str2)`
> 获取str1开始的连续多少个字节都不包括str2中的字符

## `char *strerror(int errnum)`
> 将系统错误号errno转换为便于理解的字符串输出

## `size_t strlen(const char *str)`
> 计算字符串的长度，不包括结束符

## `char *strpbrk(const char *str1, const char *str2)`
> 检索字符串str2中第一个在str1中出现的字符

## `size_t strspn(const char *str1, const char *str2)`
> 检索字符串str1中第一个不在str2中的字符

## `char *strstr(const char *haystack, const char *needle)`
> 查找字符串haystack中第一次出现字符串needle的位置

## `char *strtok(char *str, const char *delim)`
> 使用delim分割字符串str


