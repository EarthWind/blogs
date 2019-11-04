# vector
`#include <vector>`
> verctor是自动分配的连续的内存空间，可以进行扩张，但也有最大值的限制

## 方法
#### 元素访问
- `at(index)`
> 随机获取指定索引的元素，并对越界访问进行检查
```c
vector<int> v = {7, 5, 16, 34, 3, 4};
cout << "v.at(0) = " << v.at(0) << endl;
cout << "v.at(6) = " << v.at(6) << endl;
/**
v.at(0) = 7
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check
Aborted
**/
```
- `operator[]`
> 随机获取指定索引的元素
```c
vector<int> v = {7, 5, 16, 34, 3, 4};
cout << "v[2] = " << v[2] << endl;
cout << "v[7] = " << v[7] << endl;

/**
v[2] = 16
v[7] = 0
**/
```

- `front()`
- `back()`
- `data()`
> 分别获取头节点、尾节点元素以及指向头节点的指针
```c
vector<int> v = {7, 5, 16, 34, 3, 4};
cout << "v.front() = " << v.front() << endl;
cout << "v.back() = " << v.back() << endl;
cout << "v.data() = " << *v.data() << endl;

/**
v.front() = 7
v.back() = 4
v.data() = 7
**/
```

#### 迭代器
- `begin()`
- `cbegin()`


- `end()`
- `cend()`


- `rbegin()`
- `rcbegin`

- `rend()`
- `rcend()`

#### 容量
- `empty()`

- `size()`

- `max_size()`

- `reserve()`

- `capacity()`

- `shrink_to_fit()`


#### 修改器
- `clear()`

- `insert()`

- `emplace()`

- `erase()`

- `push_back()`

- `emplace_back()`

- `pop_back()`

- `resize()`

- `swap()`

