# Ceph架构-2
###### 2.12 计算PGID
当客户端和monitor通信的时候，monitor只返回给了client一个cluster map的副本，包括了集群中节点和进程的信息，monitor并没有明确告知client对象的存储位置，所以client需要使用自身拥有的信息计算出对象的存储存储位置，这些信息包括了：对象的名称，对象名称的hash值，Pool中的PG数量，cluster map；计算过程如下：
- 客户端输入信息为存储池的名称和对象id
- 对对象ID进行hash得到hash code
- 使用hash code对存储池中PG的数量取模，得到PGid
- 通过存储池名称得到存储池的ID POOLid
- 将POOLid和PGid组合得到PGID

上面的计算步骤在client上执行，并且Ceph运行Client直接和主OSD通信，可以直接将数据写入主OSD或从主OSD获取数据；

###### 2.12 Peering和Sets
之前谈到OSD会检查自身和其他OSD的状态并定时报告给monitor,注意这里其他OSD并不一定是集群中的所有osd，它们是和该当前OSD peering的OSD，peering是使同一个PG内的所有OSD上的对象的状态统一一致的过程，在一个集群中，至少要保证存在一个副本，即两份数据，此时，会组成PG的一个acting set.第一个OSD称为主OSD(Primary OSD),它会负责所有以它为主OSD的PG的peering过程，并且它(作为主OSD)是唯一一个接收来自客户端初始化写入的OSD。

###### 2.13 数据从均衡(rebalancing)
当集群中增加新的OSD时，cluster map会发生变化，则在PGID计算的结果就会由变化，这会导致发生数据迁移，CRUSH算法会在集群发生变化时最小化数据迁移，减轻集群负载；

![rebalancing](http://docs.ceph.com/docs/master/_images/ditaa-b31e1f646135b9706000fa0799d572563dffac81.png "rebalancing")

###### 2.14 数据一致性
OSD会定时清洗PG内的对象，保证对象和副本之间的一致；

###### 2.15 纠删码存储池(ERASURE CODING)
纠删码存储池将一个对象分为K+M块进行存储，类似于RAID5，其中将数据分为K个数据块和M和编码块进行存储，池的size大小为K+M，每个块的顺序记录在对象的属性中；

###### 2.16 纠删码数据的读取和写入
参考[这里](http://docs.ceph.com/docs/master/architecture/#erasure-coding "encoded pool")；

如图，在一个K=3 M=2的纠删码池中，客户端向Ceph集群写入一个名为NYAN的对象，主OSD接收对象的数据内容后，简单的将对象切分成3个数据块，第一个数据块的内容为ABC，第二个为DEF，第三个为GHI，如果对象内容不是K的整数倍，则会进行填充，另外会生成两个编码块，内容分别为YXY和QGC，具有相同名字的对象被分块存储在了不同的OSD中，数据块的顺序会作为对象属性(shard_t)进行存储；客户端从集群中读取对象时，向主OSD发送请求，主OSD分别从其他OSD上获取对象的数据块后合并成完整对象返回给客户端；

![encoded pool reading and writing](http://docs.ceph.com/docs/master/_images/ditaa-96fe8c3c73e5e54cf27fa8a4d64ed08d17679ba3.png "encoded pool reading and writing")


###### 2.17 中断全写
在纠删码存储池中，up set中的主OSD负责接收所有来自客户端的数据，并将数据编码为K+M块，让后将每个数据块发送给其他的OSD，它也负责维护归置组的权威日志；
![interrupted full write](http://docs.ceph.com/docs/master/_images/ditaa-a60e808835cf8860e19b9f2a9c83691c2a4f0218.png "interrupted full write")

如图，为K=2 M=1的纠删码池，其中有一个对象，被分成了两个数据块D1V1和D1V2，分别存储在OSD1和OSD2上，编码块C1V1存储在OSD3上，每个OSD上归置组的日志时相同的。当主OSD接收来之客户端的该对象的全覆盖写时，也就是该对象的数据会被全部替换，而不是覆盖部分，主OSD会将数据分成两块和一个编码块，如下图所示，数据的版本由v1变为v2，但数据写入成功后，其日志也相应的发送改变用于记录:

![interrupted full write](http://docs.ceph.com/docs/master/_images/ditaa-513e0558c5877884d43ffc9e7b792a5f77466831.png "interrupted full write")

由于各个OSD之间的数据写是异步的，所有写OSD上的数据写入完成后也许有的数据还没有写入完成，比如上图的C1v2和D2v2；如果集群在这种情况下没有发生异常，则最终OSD2上的数据和日志都变为v2,如下图所示：

![interrupted full write success](http://docs.ceph.com/docs/master/_images/ditaa-8db474f2d1f9a795067c4aef26c0530072cfa77f.png "interrupted full write success")

最后，各个OSD上会将第一个版本的数据块删除：

![remove data chunk](http://docs.ceph.com/docs/master/_images/ditaa-4f9c14e4c28edb4b34e0eaa33253231a4d96b11f.png "remove data chunk")

如果在OSD数据没有写入之前，OSD1被剔除了集群，此时，对于v2的数据就旨在OSD3上存在，OSD2还有写入成功，OSD1的数据全部丢失，而一个编码块是不能恢复出v2的数据，只有至少存在两个数据块时才能恢复，经过CRUSH算法后OSD4成为了主OSD，并查看last_complete_log的日志为1,则1将会成为归置组的权威日志，通过OSD上的D2v1和OSD3上C1v1恢复出D1v1并存储在OSD4上，然后将OSD3上的C1v2删除并恢复权威日志为1；

![interrupted full write failed](http://docs.ceph.com/docs/master/_images/ditaa-77b8a9b262ce5e9cbd7030c5da9ed7ab0edffc8a.png "interrupted full write failed")

#### 3. 附录
###### 3.1 参考
[1]. [Ceph document](http://docs.ceph.com/docs/master/architecture/ "Ceph document")
