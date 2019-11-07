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
> 获取头迭代器
- `end()`
- `cend()`
> 获取尾迭代器
- `rbegin()`
- `crbegin()`
> 获取反向头迭代
- `rend()`
- `crend()`
> 获取方向尾迭代器
```c
vector<int> v = {7, 5, 16, 34, 3, 4};
cout << "vectore itertor" << endl;
for(auto it = v.begin(); it != v.end(); it++){
    cout << *it << " ";
}
cout << endl;

for(auto it = v.cbegin(); it != v.cend(); it++){
    cout << *it << " ";
}
cout << endl;

for(auto it = v.rbegin(); it != v.rend(); it++){
    cout << *it << " ";
}
cout << endl;

for(auto it = v.crbegin(); it != v.crend(); it++){
    cout << *it << " ";
}
cout << endl;

/**
vectore itertor
7 5 16 34 3 4
7 5 16 34 3 4
4 3 34 16 5 7
4 3 34 16 5 7
**/
```

#### 容量
- `empty()`
> 判断vector是否为空
- `size()`
> 获取vector的大小
- `max_size()`
> 获取vector能够保存的元素个数的最大值
- `reserve(num)`
> 给vector预分配指定大小的空间
- `capacity()`
> 获取当前vector当前占用的空间
- `shrink_to_fit()`
> c++11引入，将vector的大小缩小至实际使用的大小
```c
vector<int> v = {7, 5, 16, 34, 3, 4};

if(v.empty()){
    cout << "v.empty() is true" << endl;
}else{
    cout << "v.empty() is false" << endl;
}

cout << "v.size() : " << v.size() << endl;
cout << "v.max_size() : " << v.max_size() << endl;
cout << "v.capacity() : " << v.capacity() << endl;

v.reserve(16);
cout << "v.size() : " << v.size() << endl;
cout << "v.max_size() : " << v.max_size() << endl;
cout << "v.capacity() : " << v.capacity() << endl;

v.shrink_to_fit();
cout << "v.size() : " << v.size() << endl;
cout << "v.max_size() : " << v.max_size() << endl;
cout << "v.capacity() : " << v.capacity() << endl;

/**
v.empty() is false
v.size() : 6
v.max_size() : 4611686018427387903
v.capacity() : 6
v.size() : 6
v.max_size() : 4611686018427387903
v.capacity() : 16
v.size() : 6
v.max_size() : 4611686018427387903
v.capacity() : 6
**/
```


#### 修改器
- `clear()`
> 清空vector
```c
vector<int> v = {7, 5, 16, 34, 3, 4};
cout << "clear vector" << endl;
v.clear();
cout << "v.size() : " << v.size() << endl;

/**
clear vector
v.size() : 0
**/
```


- `insert()`
> 往迭代器之前插入一个元素或两个迭代器范围内的元素；

- `emplace()`
> 插入元素，但是不产生临时对象
```c
vector<string> v5;
v5.emplace(v5.begin(), "emplace string1");
v5.emplace(v5.begin(), "emplace string2");
for(auto it: v5) {
    cout << it << endl;
}

/**
emplace string2
emplace string1
**/
```

- `erase()`
> 删除某个元素或某段位置内的元素，会造成数据的前移
```c
vector<int> v4 = {1, 2, 3, 4, 5};
int i = 1;
v4.erase(v4.begin(), v4.end());
cout << "v4.size() : " << v4.size() << endl;

/**
v4.size() : 0
**/
```

- `push_back()`
- `pop_back()`
> 添加元素到尾部或弹出
```c
cout << "push_back()" << endl;
v.push_back(1);
for(auto it : v) {
    cout << it << " ";
}
cout << endl;
cout << "v.size() : " << v.size() << endl;

cout << "pop_back()" << endl;
v.pop_back();
cout << "v.size() : " << v.size() << endl;

/**
push_back()
1
v.size() : 1
pop_back()
v.size() : 0
**/
```
```c
vector<string> v1;
v1.push_back("string1");
string s = "string2";
v1.push_back(move(s));
for(auto it = v1.begin(); it!= v1.end(); it++) {
    cout << *it << endl;
}
cout << "v1 print done" << endl;

v1.pop_back();
for(auto it: v1) {
    cout << it << endl;
}
cout << "v1 print done" << endl;

/**
string1
string2
v1 print done
string1
v1 print done
**/
```

- `emplace_back()`
> 再vector末尾添加元素，不引入临时变量
```c
vector<string> v2;
v2.emplace_back("emplace_back string1");
for(auto it: v2) {
    cout << it;
}
cout << endl;

/**
emplace_back string1
**/
```


- `resize()`
> 修改vector能够存储元素的最大值，及size()的大小

- `swap()`
> 赋值vector，并将内部清空
```c
vector<string> v2;
v2.emplace_back("emplace_back string1");
v2.push_back("push_back string2");
vector<string> v3;
v3.swap(v2);
for(auto it: v3){
    cout << it << endl;
}
cout << "print v3 done" << endl;
cout << "v2.size() : " << v2.size() << endl;

/**
emplace_back string1
push_back string2
print v3 done
v2.size() : 0
**/
```

