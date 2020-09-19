# deque
`#include <deque>`

verctor是自动分配的连续的内存空间，容器可以向前或向后扩充，而vector只能向后扩充；

## 方法
#### 成员函数
- `operator=`
- `assgin()`
- `get_allocator()`

#### 元素访问
- `at(index)`
- `operator[]`
- `front()`
- `back()`

#### 迭代器
- `begin()`
- `cbegin()`
- `end()`
- `cend()`
- `rbegin()`
- `rcbegin()`
- `rend()`
- `rcend()`

#### 容量
- `empty()`
- `size()`
- `max_size()`
- `shrink_to_fit()`

#### 修改器
- `clear()`
- `insert()`
- `emplace()`
- `erase()`
- `push_back()`
- `emplace_back()`
- `push_front()`
- `emplace_front()`
- `pop_back()`
- `pop_front()`
- `resize()`
- `swap()`

