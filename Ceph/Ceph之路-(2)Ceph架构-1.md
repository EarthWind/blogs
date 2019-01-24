# Ceph架构-1
#### 1. 架构图
![Ceph architecture](http://docs.ceph.com/docs/master/_images/stack.png "Ceph architecture")

Ceph可以在单一的集群中提供块、对象和文件存储服务，通常一个生产集群可以包含成千上万个节点，每个节点之间互相通信并动态的存储和重分布数据，如上图所示，Ceph架构主要包括了三层：
- 对象存储层RADOS： Ceph集群中的数据最终都会被保存为RADOS中的对象，该层为Ceph提供了大数据数据存储能力、高可用和分布式特性；
- 接口层LIBRADOS： 该层主要抽象了RADOS层的接口，提供了C/C++及Python等接口用来开发上层应用；
- 应用层： Ceph提供的RADOSGW，RBD以及CephFS都属于该层，它们都使用了LIBRADOS提供的接口来提供相应的服务；

#### 2. 存储集群
###### 2.1 存储集群
一个集群主要由两类进程组成：
- Ceph Monitor: 主要维护cluster map，客户端在和集群交互的时候需要先从Monitor获取cluster map。在生产环境中需要确保Monitor的高可用；
- Ceph OSD Daemon: 用来存储数据，直接处理客户端的读写请求，并且检查自身和其他OSD的状态定时更新给Monitor，这样Monitor可以获取集群的视图和个节点的状态；

> Ceph MGR在mimic版本也是必须运行的进程，用来监控集群状态；

> 数据存储原理： Ceph Client和Ceph OSD能够通过cluster map和CRUSH算法计算出数据的存储位置，而不依赖中间查询表；

###### 2.2 数据存储
客户端(RBD, CephFS, RGW或使用LIBRADOS实现的应用)在通过cluster map和CRUSH计算出数据存储的OSD后，会直接和OSD通信，OSD直接处理客户端的读写请求。OSD存储数据是以对象为单位的，默认大小为4MB，在OSD使用filestore作为后端存储的时候，每个对象对应操作系统上的一个文件，OSD中的对象没有层级关系，是在一个平行的域名空间中(flat namespace),每个对象包含以下三个部分：
- *对象ID： RADOS中唯一标识*
- *对象数据：对象内容*
- *对象元数据： 对象的创建时间，权限，修改时间等信息*

![osd object](http://docs.ceph.com/docs/master/_images/ditaa-ae8b394e1d31afd181408bab946ca4a216ca44b7.png "osd object")

###### 2.3 扩展性和高可用性
在一些复杂的系统中，会依赖中心化组件，这会限制了系统的扩展能力并导致新能瓶颈；Ceph不存在中心的组件，Client可以和OSD直接通信，对象会被复制保存在多个OSD中确保数据的安全和高可用性，Monitor也支持多活高可用性；

> CRUSH算法使Ceph集群消除了中心化组件；

###### 2.4 CRUSH简介
Client和OSD都可以基于cluster map上运行CRUSH算法计算出对象的存储位置，这样可以有效的将负载平均分布到集群中而不依赖单个节点，比起传统的中心化查询表方式，Ceph更适合超大规模的存储,可以操作文末扩展阅读部分查看算法论文；

###### 2.5 集群图(cluster map)
在上一篇博文中已经介绍了cluster map.主要包括由五类map组成：
- The Monitor Map
- The OSD Map
- The PG Map
- The CRUSH Map
- The MDS Map

每一类map都保存了自身的更新迭代历史，即当前版本，创建时间，最近修改时间，map内元素状态等信息；

###### 2.6 Monitor高可用
Client在读取或写入数据之前，需要先和Monitor通信获取最新的cluster map,这样才能保证通过CRUSH算法计算出的存储节点时正确的，Monitor可以单节点运行，不过这会导致Monitor的单节点失效问题，如果该Monitor down掉了，那客户端就不能进行读写操作了。因此，生产环境中必须保证Monitor的高可用性，多个Monitor的引入会导致有可能有些Monitor的cluster map不是最新的，如果将该cluster map传输给客户端，也会导致计算错误，因此Ceph引入了Paxos算法保证monitor之间集群状态的一致性；

###### 2.7 认证高可用
为了防止中间人攻击，Ceph引入了cephx对Ceph用户和进程(daemon)进行认证,每个monitor都可以进行认证和分发密钥，所以使用cephx不会给Ceph集群带来单点失效问题或扩展瓶颈；
![ceph create user](http://docs.ceph.com/docs/master/_images/ditaa-6b1dafb6d8f177ab2beb3325857f1e98e4593ec6.png "ceph create user")

如图，Ceph在使用```ceph auth get-or-create```命令创建用户时，monitor会生成一个密钥并保存，同时将该密钥返回给客户端；

![ceph client authenticate](http://docs.ceph.com/docs/master/_images/ditaa-56e3a72e085f9070289331d64453b84ab1e9510b.png "ceph client authenticate")

客户端使用创建的用户和monitor通信时，monitor会返回一个加密的session key,客户端使用共享密钥对session key进行解密并获取原始session key,之后客户端使用session key进行一个ticket请求，monitor会返回一个有secrect key加密的ticket,客户端使用这个ticket和OSD或元数据服务器进行通信；

![ceph authenticate](http://docs.ceph.com/docs/master/_images/ditaa-f97566f2e17ba6de07951872d259d25ae061027f.png "ceph authenticate")

如上图所示，是客户端认证的所有过程；
> 需要注意的是，cephx只保证ceph client到Ceph集群之间的数据安全认证，用户主机到ceph client之间的安全措施是在cephx的范畴之外的；

###### 2.8 智能化的进程(SMART DAEMONS)
Ceph消除了集中化组件带来的瓶颈和性能限制，集群中的所有进程之间都可以直接相互通信，因此OSD可以轻松的使用CPU和RAM来执行任务，这给Ceph集群带来了几个好处：
- OSD和Client直接通信
- OSD之间能够互相确认状态： OSD会检查自身和其他OSD的状态并定时报告给Monitor,这减轻了Monitor进程的负载(轻量级)；
- 数据清洗： 数据清洗是Ceph为了确保数据一致性和清洁度所做的定时任务，分为轻度清洗和深度清洗，轻度清洗只会比较主对象和其他副本对象之间元数据是否一致，通常一天清洗一次；深度清洗则会按bit校验数据是否一致，一周清洗一次；
- 数据复制：在Ceph存储数据时，client使用CRUSH算法计算对象的存储位置，先映射到pool和pg中，让后查询CRUSH MAP找出PG的主OSD，然后客户端把数据直接发送给主OSD；主OSD收到客户端的发送来的对象数据后，会使用自身的CRUSH MAP找出副本存储的OSD，然后把对象数据发送给副本OSD，当对象写入成功后再回复客户端数据写入的状态；
![data replication](http://docs.ceph.com/docs/master/_images/ditaa-54719cc959473e68a317f6578f9a2f0f3a8345ee.png "data replication")

###### 2.9 动态集群管理
Ceph具有动态集群管理的关键设计为自主、自我修复和智能OSD守护进程，而这些都得益于CRUSH算法的使用；


###### 2.10 POOL
Ceph存储支持“池”的概念，是数据存储的逻辑分区；对象的副本数量、CRUSH规则和PG数量都是针对池来设定的；如图是客户端向集群写入数据的逻辑过程；

![ceph pool](http://docs.ceph.com/docs/master/_images/ditaa-778c68aa73d182d88236b73e1a4a40bd78fb32d6.png "ceph pool")

###### 2.11 PG映射到OSD
Ceph客户端和Ceph OSD之间存在一个映射隐藏层，CRUSH算法会将对象映射到PG，然后再将PG映射到一个或多个OSD中，PG则是映射中的隐藏层，它使得Ceph集群能够动态的增加和移除OSD，并在这种场景中最小化数据迁移，如图是CRUSH算法的映射过程：
![crush map](http://docs.ceph.com/docs/master/_images/ditaa-c7fd5a4042a21364a7bef1c09e6b019deb4e4feb.png "crush map")

#### 3. 附录
###### 3.1 参考
[1]. [Ceph document](http://docs.ceph.com/docs/master/architecture/ "Ceph document")

###### 3.2 扩展阅读
[1]. [RADOS - A Scalable, Reliable Storage Service for Petabyte-scale Storage Clusters.](https://ceph.com/wp-content/uploads/2016/08/weil-rados-pdsw07.pdf "RADOS - A Scalable, Reliable Storage Service for Petabyte-scale Storage Clusters.")

[2]. [CRUSH - Controlled, Scalable, Decentralized Placement of Replicated Data.](https://ceph.com/wp-content/uploads/2016/08/weil-crush-sc06.pdf "CRUSH - Controlled, Scalable, Decentralized Placement of Replicated Data.")

[3]. [分布式系统Paxos算法](https://www.jdon.com/artichect/paxos.html "分布式系统Paxos算法")
