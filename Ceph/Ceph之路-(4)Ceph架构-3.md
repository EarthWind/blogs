# Ceph架构-3
###### 2.18 缓存层(CACHE TIERING)
缓存层能为后端存储层存储的部分数据提供更好的客户端IO性能，缓存层一般由相对快和昂贵的存储设备(比如固态存储)组成的池配置而成，而后端存储层则由相对慢和便宜的设备或纠删码池组成，如图，objector负责处理对象的存储位置，缓存层代理决定什么时候将缓存层中的数据刷入后端存储层；

![cache tiering](http://docs.ceph.com/docs/master/_images/ditaa-2982c5ed3031cac4f9e40545139e51fdb0b33897.png "cache tiering")

###### 2.19 扩展ceph
可以创建名为```Ceph Classes```的共享类来扩展Ceph,Ceph会动态的加载放置在 ```osd class dir```目录下.so文件;

#### 3. Ceph协议
Ceph客户端使用内置的协议和Ceph集群交互，Ceph将此协议打包到了librados库中，所以用户可以使用librados自定义自己的客户端，下图时一个基本的架构：

![ceph protocol](http://docs.ceph.com/docs/master/_images/ditaa-1a91351293f441ce0238c21f2c432331a0f5a9d3.png "ceph protocol")

###### 3.1 内置协议和librados
Ceph存储集群提供了一个简单具有异步通信能力的对象存储接口，它能直接并行的访问集群中的所有对象，librados提供给了以下的功能：
- 对池进行操作
- 快照和写时复制克隆
- 读写对象/创建和删除对象/整个对象或按字节范围的访问/追加和截断数据
- 创建/设置/获取/删除XATTRs属性
- 创建/设置/获取/删除键值对
- 复合运算和双重ack定义(Compound operations and dual-ack semantics)
- 对象类(object classes)

###### 3.2 对象监视/通知
客户端可以持久性的监视一个对象，当客户端发送一个通知消息和一个数据给所有的监视者(watcher)，监视者收到后会发送一个通知给该客户端，这种特性可以让客户端使用任何一个对象作为同步通信通道；

![object watch notify](http://docs.ceph.com/docs/master/_images/ditaa-afd50e13a81128d0a2c38fadcd27dfc8b7ac523b.png "object watch notify")

###### 3.3 数据条带化(stripping)
存储设备存在带宽限制，这会影响系统的性能和扩展性，因此存储系统通常支持条带化，将连续的信息存储在多个设备上，以此来增加系统的性能和带宽；最常见的数据条带化方式就时RAID，和Ceph条带化最相似的是RAID0；但由于ceph的多副本特性，Ceph条带化除了具有RAID0的带宽之外，也具有n路RAID镜像快速恢复数据的特性；
Ceph提供三种类型的客户端：RBD,CephFS和RGW(Ceph object storage)。当客户端需要写入数据，会将用户提供的数据格式转换为对象，以便Ceph集群进行存储；

> Ceph集群中存储的对象是没有条带化的，Ceph客户端需要自己将要保存的数据条带化到多个对象，然后调用librados库并使用并行IO，也就是说数据的条带化需要客户端来执行和维护；

最简单的条带化方式就是首先将条带化单元不断的往一个对象里面写，直到对象写满为止，然后创建第二个对象，继续写入条带化单元，以此类推，写完数据为止；但这种方式没有完全的使用Ceph多归置组并行分发数据的特性，对于小数据这种方式是有效的，但不会带来很大的性能提升，如下图:

![ceph stripping](http://docs.ceph.com/docs/master/_images/ditaa-deb861a26cf89e008006b63d95885b4ed88ba608.png "ceph stripping")

如果客户端有很大的块设备，大size的对象，比如像视频，或是CephFS催在大目录，那么就可以考虑将对象数据条带化到多个对象，这些对象可以映射到不同的PG和OSD上，Ceph可以并行的写入以此来提升性能，获取更快的读写速度；如下图所示，客户端数据被条带化到由4个对象组成的对象集合中，条带化单元会顺序的写入不同对象，直到该对象集合写满以后创建另一个对象集合，以此类推，如下图所示：

!["ceph stripping large object"](http://docs.ceph.com/docs/master/_images/ditaa-92220e0223f86eb33cfcaed4241c6680226c5ce2.png "ceph stripping large object")

由以下三个因素会影响Ceph的条带化：
- Object size: Ceph集群的对象大小，一般默认为4MB；
- Stripe Width: 即条带化单元的大小，Ceph客户端会将数据条带化为大小相同的条带化单元，可能最后一个单元的大小会有区别，该值应应该小于对象大小，才能让一个对象包含多个条带化单元；
- Stripe Count: 即对象集合(object set)中对象的个数；

条带化的数据会被写入不同的对象，不同的对象会被CRUSH算法映射到不同的PG和OSD上，但是这些对象都存储在同一个池中，所以条带化后的数据使用相同crush map和访问控制；

> 已经条带化的数据和写入集群的对象是不能在进行改变的；

#### 4. Ceph客户端
Ceph包含了一些服务接口：
- 块设备(Block Devices): Ceph Block Devices(RBD)服务提供大小可变，具有快照和克隆功能的块设备；Ceph可以将块设备条带化到整个集群来提高性能，Ceph块设备支持内核对象(kernel objects: KO)和使用librbd的QEMU虚拟机使用，使用librbd方式可以避免虚拟化系统内核对象开销；
- 对象存储(Object Storage)：Ceph Object storage(RGW)提供了兼容S3和Openstack Swift接口的RESTful API；
- 文件系统(Filesystem): Ceph Filesystem(CephFS)兼容POSIX
如图是客户端架构：

![client architecture](http://docs.ceph.com/docs/master/_images/ditaa-804f45fe5a789fa161d7b4100740adf992a0dc07.png "client architecture")

###### 4.1 Ceph对象存储
Ceph的对象存储进程是radowsgw,一个使用RESTful风格存储对象和元数据的HTTP API，它位于Ceph存储集群之上，拥有自己的数据格式，能够独立管理自己的用户、用户认证和访问控制，RADOS网关使用统一的命名空间，并且它兼容S3和OpenStack swift接口，所以使用S3接口写入的数据可以被使用swift接口的应用读取出来；

###### 4.2 Ceph块设备
RBD会将块设备镜像条带化到集群中的多个对象中，每个对象又分散地映射到归置组(PG)中,归置组分布于集群中OSD进程上;具有精简快照特性的块设备通常用于虚拟化和云计算解决方案中，像Openstack和Cloudstack使用libvirt、librbd以及QEMU来使用Ceph RBD作为后端存储；但是Ceph现在还不支持其他虚拟机管理程序，比如Xen,使用librbd来使用Ceph,需要使用内核对象的方式来使用；

###### 4.3 Ceph文件系统
CephFS提供基于Ceph对象存储之上兼容POSIX的文件系统服务，CephFS中的文件会被映射为集群中的对象进行存储，客户端可以将CephFS挂载为内核对象或作为用户空间文件系统使用；如图是CephFS逻辑架构图：

![CephFS](http://docs.ceph.com/docs/master/_images/ditaa-9f5c6a9a2a7cc576498e3922cb03fafed4347fb9.png "CephFS")

CephFS包括了部署在集群中的MDS(Ceph Metadata Server),它的作用是将将文件系统元数据(目录，文件所属者，文件访问权限等)驻留在内存中，这样在对文件系统目录(ls)和更换目录(dd)时不会访问OSD，不仅减轻了OSD的负载，还提高了文件系统的性能；
- 高可用性： MDS可以运行在是主备模式，monitor会自动触发standby进程取代出现问题的主MDS进程，而用于恢复的所有数据和日志都存储在Ceph集群中，不会应为主OSD的问题而丢失；
- 扩展性： 多活MDS会将目录树切分为子树而有效的均分负载；

MDS的主备模式和多活模式是可以同时运行的；

#### 5. 附录
###### 5.1 参考
[1]. [Ceph document](http://docs.ceph.com/docs/master/architecture/ "Ceph document")

