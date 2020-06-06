# CRUSH
## 简介
> CRUSH(Controlled Replication Under Scalable Hashing)是一种伪随机数据分布算法，Ceph集群通过CRUSH算法来确定数据的存储位置以及如何检索数据,根据每个设备的权重尽可能概率平均地分配数据。客户端的运用自身计算资源，给定一个输入x后，CRUSH使用强大的多重整数hash函数根据集群map,定位规则输出一个确定的有序的存储目标向量R,客户端便可直接连接OSD进行数据的传输,而非通过一个中央服务器查表搜索，使得Ceph避免了单点故障，性能瓶颈和伸缩性的物理限制， CRUSH主要存在一下几个优点：
> - 任何组件都可以通过CRUSH算法计算出对象的存储位置
> - 很少的元数据(cluster map)，只要当删除添加设备时，这些元数据才需要改变。
>
> CRUSH需要使用集群的CRUSH MAP,该MAP包括：
> - OSD列表
> - BUCKET列表
> - 数据复制规则表

## 操作
#### 获取CRUSH MAP
```SHELL
ceph osd getcrushmap -o crush.dump
crushtool  -d crush.dump –o crush.txt
```

#### CRUSH MAP内容
> CRUSH MAP主要包括五个主要部分：
> - Tunables:
> - devices: 设备列表，是CRUSH MAP树的叶子节点，对应了OSD磁盘
> - Bucket类型：定义了在CRUSH MAP的层次结构中所用到的Bucket类型
> - Bucket实例：该部分声明了host的Bucket实例，和其他可选的失效域
> - Rules：定义了选择在数据存储和检索时选择Bucket的规则
```SHELL
# 查看池的crush规则
$ ceph osd pool get test crush_rule
crush_rule: replicated_rule # 表示使用该存储池进行数据存储的crush规则为replicated_rule
```
```SHELL
# begin crush map
tunable choose_local_tries 0 # 已废弃，为做向后兼容应保持为 0
tunable choose_local_fallback_tries 0 # 已废弃，为做向后兼容应保持为 0
tunable choose_total_tries 50 # 可调，选择bucket的最大尝试次数，缺省值为50，已被验证是比较合理的值,如果50还无法选择出期待的bucket，可以适当调整这个参数
tunable chooseleaf_descend_once 1 # 已废弃，为做向后兼容应保持 1
tunable chooseleaf_vary_r 1 # 这个参数是为了fix一些bug而存在，应该保持为1
tunable chooseleaf_stable 1 # 这个参数的实现也是为了避免一些不必要的pg迁移，应该保持为1
tunable straw_calc_version 1 # straw 算法的版本，为了向后兼容这个值应该保持为 1
tunable allowed_bucket_algs 54 # 允许使用的bucket 选择算法

# devices # OSD列表
device 0 osd.0 class ssd # 格式为：device 设备ID OSDID class 磁盘类型，是CRUSH MAP树的叶子节点,用于代表可以存储数据单个OSD进程。注意设备ID时非负整数，在CRUSH MAP中>=0表示OSD，<0代表Bucket;
device 2 osd.2 class ssd

# types, 中间节点Bucket类型定义，大于等于0
type 0 osd # 叶子Bucket type 0,可以指定任何名字；0:类型ID，osd:类型名称，可以修改为任意名字；一般编号较高的Bucket包含编号较低的Bucket，并且type 0一般表示叶子节点；
type 1 host
type 2 chassis
type 3 rack
type 4 row
type 5 pdu
type 6 pod
type 7 room
type 8 datacenter
type 9 region
type 10 root

# buckets, Bucket实例，Bucket类型必须使用，这里的类型为host
host mon {
        id -3                   # Bucket的ID号，一个bucket在crush map 实际存储位置是 buckets[-1-(bucket id)]
        id -4 class ssd         # do not change unnecessarily
        # weight 0.873
        alg straw2              # 表明该Bucket在选择低一级Bucket时所使用的算法，
        hash 0                  # 每个Bucket使用的hash算法，hash 0表示使用rjenkins1算法，当前只有这样一种选择，哈希算法的输出为32位
        item osd.2 weight 0.218 # item声明了该Bucket所包含的低一级Bucket，以及低一级Bucket的所有item的权重之和；另外，官方推荐使用1.0作为存储容量为1T的设备的权重，相应的权重为0.5代表500G的存储设备；
        item osd.3 weight 0.218
        item osd.4 weight 0.218
        item osd.0 weight 0.218
}
host osd {
        ...
}
root default { # 默认的根Bucket
        id -1
        id -2 class ssd
        alg straw2
        hash 0
        item mon weight 0.873
        item osd weight 0.873
}

# rules，定义了归置，分布或复制策略，可以为不同的pool设置不同的规则；crush做的每一步都是按照rule规则来执行
rule replicated_rule { # 默认的复制类型的pool的规则，replicated_rule为规则名称；
        id 0  #规则ID
        type replicated # pool的类型
        min_size 1 # 定义CRUSH使用该规则时PG副本的最小值
        max_size 10 # 定义CRUSH使用该规则时PG副本的最大值
        step take default #选择Bucket名称，并迭代到CRUSH MAP树的底部，这里选择了名称为default的root Bucket为树的根开始；choose选择bucket后就结束，chooseleaf则会在选出bucket后递归到odd的选择，一般用在主机节点即osd节点的上以层级；
        step chooseleaf firstn 0 type host # 选择host类型的Bucket,firstn可能的取值时indep，firstn尽可能的返回指定数量的bucket，如果不能满足需要的数量，就返回查找到的数量，而indep则会返回所需的数量，但某些某些元素时不可用的；0表示选择数量为存储池的副本数；0<num<pool-num-replicas,表示就选择num个Bucket,num<0表示选择pool-num-relicas-{num}Bucket;需要注意的是select开始的起点，是上一个step的输出；
        step emit # 输出选择结果并清空栈，通常应用于规则末尾；
}
rule erasure-code { # 默认的纠删码类型的pool的规则，erasure-code为规则名称；
        ... 
}
```

## Bucket选择算法
> Bucket的使用的选择算法有如下几种：
> - uniform:当所有的设备拥有相同的权重时可使用该类算法；
> - list:该类型的Bucket把他们的内容汇聚为链表；
> - tree:该类型的Bucket是一种二进制搜索树；
> - straw:抽签类型的Bucket,考虑权重；
> - straw2:优化的抽签算法
```C++
enum crush_algorithm {
	CRUSH_BUCKET_UNIFORM = 1,
	CRUSH_BUCKET_LIST = 2,
	CRUSH_BUCKET_TREE = 3,
	CRUSH_BUCKET_STRAW = 4,
	CRUSH_BUCKET_STRAW2 = 5,
};
```

| Action | uniform | list | tree | straw | straw2 |
| --- | --- | --- | --- | --- | --- |
| 算法复杂度 | O(1) | O(n) | O(logn) | O(n) | O(n) |
| 增加存储节点 | 差 | 最优 | 好 | 最优 | 最优 |
| 删除存储节点 | 差 | 差 | 好 | 最优 | 最优 |

#### uniform算法
```C++
struct crush_bucket_uniform {
       struct crush_bucket h; /* 指向  */
	__u32 item_weight;  /*!< 16.16 fixed point weight for each item */
};

struct crush_bucket {
	__s32 id;        /*!< bucket identifier, < 0 and unique within a crush_map */
	__u16 type;      /*!< > 0 bucket type, defined by the caller */
	__u8 alg;        /*!< the item selection ::crush_algorithm */
        /*! @cond INTERNAL */
	__u8 hash;       /* which hash function to use, CRUSH_HASH_* */
	/*! @endcond */
	__u32 weight;    /*!< 16.16 fixed point cumulated children weight */
	__u32 size;      /* 该节点包含的子节点的数量，如果为0，则表示不包含子节点 */
        __s32 *items;    /* 将子节点保存在数组中，这是子节点的节点ID号，bucket的ID为负数，设备的ID为正数 */
};
```
> - 该类别算法适用于所有子节点权重相同的情况，而且 bucket 很少增加或删除 item,这时的查询 速度是最快的，因为该类 bucket 在选择子节点的时候不考虑权重问题，全部随机选择，所以权 重并不会影响选择结果； 
> - 适用于子节点变化概率小的情况，在 size 发生变化时，perm 数组会完全重新排列，也就意味着 保存在子节点的所有数据都要发生重排，造成数据迁移； 

#### list算法
```C++
struct crush_bucket_list {
        struct crush_bucket h; /*!< generic bucket information */
	__u32 *item_weights;  /*!< 16.16 fixed point weight for each item */
	__u32 *sum_weights;   /*!< 16.16 fixed point sum of the weights */
};
```

#### tree算法
```C++
struct crush_bucket_tree {
	struct crush_bucket h;  /* note: h.size is _tree_ size, not number of
				   actual items */
	__u8 num_nodes;
	__u32 *node_weights;
};
```

#### straw算法
```C++
struct crush_bucket_straw {
	struct crush_bucket h;
	__u32 *item_weights;   /* 16-bit fixed point */
	__u32 *straws;         /* 16-bit fixed point */
};
```

#### straw2算法
```C++
struct crush_bucket_straw2 {
        struct crush_bucket h; /*!< generic bucket information */
	__u32 *item_weights;   /*!< 16.16 fixed point weight for each item */
};
```


## 参考列表
- [深入理解crush(1)—-理解crush map文件](https://www.dovefi.com/post/%E6%B7%B1%E5%85%A5%E7%90%86%E8%A7%A3crush1%E7%90%86%E8%A7%A3crush_map%E6%96%87%E4%BB%B6/)
- [大话Ceph--CRUSH那点事儿](http://www.xuxiaopang.com/2016/11/08/easy-ceph-CRUSH/)

