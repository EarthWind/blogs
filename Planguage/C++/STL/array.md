# array
`#include <array>`

array是固定大小的容器，声明后不可以再修改大小，该种类型是和c数组类似的聚合类型，支持获取容器的大小、随机访问迭代器、赋值等操作，容器内可以支持任意类型的数据类型；

## 方法
- `at(index)`
获取指定下表索引的元素，会对下标越界进行检查

```c++
array<int, 3> a1{ {7, 2, 4} };
cout << "a1.at[2] = " << a1.at(2) << endl;
cout << "a1.at[4] = " << a1.at(4) << endl;

/**
a1.at[2] = 4
terminate called after throwing an instance of 'std::out_of_range'
  what():  array::at
Aborted
**/
```

- `operator[]`
获取指定下表的元素，和数组类似，不会做越界的检查

```c++
array<int, 3> a1 = {7, 2, 4};
cout << "a1[1] = " << a1[1] << endl;
cout << "a1[4] = " << a1[4] << endl;

/**
a1[1] = 2
a1[4] = 6303936
**/
```

- `front()`
获取数组的第一个元素

- `back()`
获取数组的最后一个元素

```c++
array<int, 3> a1 = {7, 2, 4};
cout << "a1.front() = " << a1.front() << endl;
cout << "a1.back() = " << a1.back() << endl;

/**
a1.front() = 7
a1.back() = 4
**/
```

- `data()`
返回指向内存中第一个元素的指针

```c++
cout << "a1.data() = " << *a1.data() << endl;

/**
a1.data() = 7
**/
```

- `empty()`
判断数组是否为空，及是否有元素

```c++
if(a1.empty())
    cout << "a1.empty() = true" << endl;
else
    cout << "a1.empty() = false" << endl;

/**
a1.empty() = false
**/
```

- `size()`
- `max_size()`
获取数组的当前元素数量和最大元素数量

```c++
cout << "a1.size() = " << a1.size() << endl;
cout << "a1.max_size() = " << a1.max_size() << endl;

/**
a1.size() = 3
a1.max_size() = 3
**/
```

- `begin()`
- `end()`
获取数组的开始和结尾迭代器

```c++
array<int, 3>::iterator it  = a1.begin();
for(; it != a1.end(); ++it)
    cout << *it << " ";
cout << endl;

/**
7 2 4
**/
```

- `rbegin()`
- `rend()`
获取反向迭代器

```c++
for(array<int, 3>::reverse_iterator it = a1.rbegin(); it != a1.rend(); ++it)
    cout << *it << " ";
cout << endl;

/**
4 2 7
**/
```
