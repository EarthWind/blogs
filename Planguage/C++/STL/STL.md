## STL的6大部件
- container: 容器
- allocator: 分配器
- algorithms: 算法
- iteratos: 迭代器
- functor: 仿函数
- adaptor: 适配器，仿函数适配器，迭代器适配器，容器适配器

## 容器分类
> 分配为3544
- sequence containers(顺序容器)
> - array(连续内存，和数组一致，大小不变)
> - vector(连续内存，大小可变，向后扩充，每次两倍扩充)
> - deque(连续内存，大小可变，双向扩充)
> - list(非连续内存，是一个双向链表)
> - forward-list(非连续内存，单项链表实现)
- associative containers(关联容器)
> - set(使用红黑树实现元素不重复元素)
> - multiset(元素可重复)
> - map(使用红黑树实现的元素不重复的kv)
> - multimap(元素可重复)
- unordered containers(不定序容器)
> - unordered set/multiset(hash map实现)
> - unordered map/multimap(hash map实现)
