# forward_list
`#include <forward_list>`

forward_list容器以单向链表存储元素，和list最大的区别是只能从头便利到尾部，不能进行反向遍历，和array,vector,deque的区别是任何位置插入数据效率更高；
- 无反向迭代器
- 没有back()成员函数
- 没有push_back()、pop_back()、emplace_back()等成员函数

## 方法
#### 成员函数
- `operator=`
- `assgin()`
- `get_allocator()`

#### 元素访问
- `front()`

#### 迭代器
- `before_begin()`
- `cbefore_begin()`
- `begin()`
- `cbegin()`
- `end()`
- `cend()`

#### 容量
- `empty()`
- `max_size()`

#### 修改器
- `clear()`
- `insert_after()`
- `emplace_after()`
- `erase_after()`
- `push_front()`
- `pop_front()`
- `emplace_front()`
- `resize()`
- `swap()`

#### 操作
- `merge()`: 将两个已经排序的容器合并为一个
- `splice_after()`
- `remove()`
- `remove_if()`
- `reverse()`: 将列表中的元素反转
- `unique()`: 去重
- `sort()`: 排序
