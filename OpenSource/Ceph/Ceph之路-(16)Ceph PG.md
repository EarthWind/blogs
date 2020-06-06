# PG
## 状态
> - creating: 存储池在创建一个或多个归置组时会显示creating状态
> - peering: 就PG对应的主osd和副osd的对象和元数据达成一致，该状态称为互联
> - active: 互联完成后PG就变为active状态
> - clean: PG中的对象数量满足了规定的副本
> - degraded: 当PG中的对象的数量暂时没有满足副本要求时，会是该状态，如果一个OSD出现了问题导致PG的状态一直是degraded状态，Ceph集群会把该OSD标记位OUT，这个时间是mon_osd_down_out_interval参数设定，默认位600s，然后把该OSD上的数据映射到其他OSD上取；
> - recovering: Ceph被设计位可容错，当OSD的硬件或软件发送变化导致OSD down掉后，其内的数据相较于其他OSD会存在差异，当OSD up后，会对该OSD上的数据进行恢复，此时PG处于recovering状态；
> - backfilling: 有新的OSD加入集群时，CRUSH会把现在集群内的部分归置组分配给他，使用回填可以使这个过程在后台运行；
> - remapped: 当PG的Acting Set变更时，数据就需要从旧集合迁移到新的集合
> - stale: OSD有一段时间没有向Monitor发送心跳信号了
> - inactive: 该状态下PG是不可以进行读写请求的
> - Scrubbing: Ceph集群正在进行一致性校验

## 参考列表
- [ceph pg state简介](http://luqitao.github.io/2016/07/14/ceph-pg-states-introduction/)
- [分布式存储Ceph之PG状态详解](https://www.jianshu.com/p/36c2d5682d87)