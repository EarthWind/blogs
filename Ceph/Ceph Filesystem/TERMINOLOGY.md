# TERMINOLOGY
> 一个Ceph集群可能没有或有多个文件系统，文件系统可以在使用命令`ceph fs new <filesystemname> meta_pool data_pool`创建时指定便于读的名称，它还有一个整数的ID，叫做文件系统集群ID，或则FSCID；
>
> 每个文件系统可以有多个rank，默认有一个，从0开始，每个rank可以理解为元数据的一部分，控制文件系统中的rank数量可以参考多活MDS部分；
>
> 每个CephFS中的ceph-mds在初始化启动的时候是没有rank值的，它需要通过Monitor来赋予，一个后台进程一次可以有一个rank，在cphe-mds进程在退出的时候才会释放掉；


