# LIBIBVERB
## 1. 获取设备信息
> - `struct ibv_device **ibv_get_device_list(int *num_devices);`
>
> 描述：获取系统上的infiniband设备列表，
>
> num_devices: 输出参数，返回设备的数量
>
> 返回：返回设备列表，程序结束时需要使用`ibv_free_device_list(struct ibv_device **list)`释放掉

> - `const char *ibv_get_device_name(struct ibv_device *device);`
>
> 描述：获取设备的设备名称，也可以通过`device -> name`获取设备名称
>
> device: 指定设备

> - `const char *ibv_node_type_str(enum ibv_node_type node_type);`
>
> 描述：将节点类型转换为字符串
>
> node_type: `device -> node_type`

> - `__be64 ibv_get_device_guid(struct ibv_device *device);`
>
> 描述：获取设备的guid
>
> device: 设备结构

> - `void ibv_free_device_list(struct ibv_device **list);`
>
> 描述：释放设备列表所占用的空间
>
> list: 指定设备列表，由`ibv_get_device_list(int *num_devices)`返回

> - `int ibv_fork_init(void);`
>
> 描述：初始化rdma结构体，在避免在fork的时候发生问题

> - `struct ibv_context *ibv_open_device(struct ibv_device *device);`
>
> 描述：打开设备

> - `int ibv_close_device(struct ibv_context *context);`
>
> 描述：关闭打开的设备

> - `const char *ibv_port_state_str(enum ibv_port_state port_state);`
>
> 描述：将端口状态转换为字符串

> - `int ibv_query_device(struct ibv_context *context,struct ibv_device_attr *device_attr);`
>
> 描述：查询端口属性
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_query_port(struct ibv_context *context, uint8_t port_num,struct ibv_port_attr *port_attr);`
>
> 描述：获取设备物理端口的属性，其中port_num从1开始
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_query_gid(struct ibv_context *context, uint8_t port_num,int index, union ibv_gid *gid);`
>
> 描述：获取端口的全局标识gid，每一个端口至少被SM(subnet manager)赋予一个gid
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_query_pkey(struct ibv_context *context, uint8_t port_num,int index, __be16 *pkey);`
>
> 描述：获取端口的分区键值(partiton key(pkey) table)，每个端口至少被SM(subnet manager)赋予一个pkey，pkey用于标识了一个端口所属的分区
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_pd *ibv_alloc_pd(struct ibv_context *context);`
>
> 描述：分配保护域，规定了QP对内存的范文权限
> 
> 返回：成功-返回保护域指针；失败-返回NULL

> - `int ibv_dealloc_pd(struct ibv_pd *pd);`
>
> 描述：释放掉保护域
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_cq *ibv_create_cq(struct ibv_context *context, int cqe, void *cq_context,struct ibv_comp_channel *channel, int comp_vector);`
>
> 描述：创建完成队列，发送队列和接收对了可以有独立的QP，也可以共享一个CQ，当然，CQ也可以在多个QP内共享
>
> context: 指定打开设备的上下文
>
> cqe: 指定创建队列时的元素最小个数
>
> cq_context: 
>
> channel: 之间通知通道
>
> 返回：成功-返回CQ的指针；失败-返回NULL

> - `int ibv_resize_cq(struct ibv_cq *cq, int cqe);`
>
> 描述：修改CQ的最小中的元素最小值
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_destroy_cq(struct ibv_cq *cq);`
>
> 描述：销毁掉完成队列
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_comp_channel *ibv_create_comp_channel(struct ibv_context *context);`
>
> 描述：创建完成队列通道，用于通知完成队列中有新元素加入
>
> 返回：成功-指向完成队列通道的指针；失败-返回NULL

> - `int ibv_destroy_comp_channel(struct ibv_comp_channel *channel);`
>
> 描述：释放完成队列
>
> 返回：成功-返回0；失败-返回-1，并设置errno

#### 保护域操作
> - `struct ibv_mr *ibv_reg_mr(struct ibv_pd *pd, void *addr,size_t length, int access);`
>
> 描述：将内存注册到保护域PD中
>
> 返回：成功-指向MR的指针；失败-返回NULL

> - `int ibv_dereg_mr(struct ibv_mr *mr);`
>
> 描述：释放掉保护域

> - `struct ibv_qp *ibv_create_qp(struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr);`
>
> 描述：创建QP
>
> 返回：成功-返回ibv_qp的指针；失败-返回NULL

> `int ibv_destroy_qp(struct ibv_qp *qp);`
>
> 描述：释放一个QP
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_srq *ibv_create_srq(struct ibv_pd *pd, struct ibv_srq_init_attr *srq_init_attr);`
>
> 描述：创建一个和指定保护域关联的共享队列
>
> 返回：成功-指向共享队列的指针；失败-返回NULL

> - `int ibv_modify_srq(struct ibv_srq *srq, struct ibv_srq_attr *srq_attr, int srq_attr_mask);`
>
> 描述：修改共享队列属性
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_destroy_srq(struct ibv_srq *srq);`
>
> 描述：释放共享队列
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_ah *ibv_create_ah(struct ibv_pd *pd, struct ibv_ah_attr *attr);`
>
> 描述：创建AH，在RC,UC传输模式下，AH被和QP关联，在UD传输模式中，AH和WR关联

> - `int ibv_destroy_ah(struct ibv_ah *ah);`
>
> 描述：释放ah

#### 启动QP
> - `int ibv_modify_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr,int attr_mask);`
>
> 描述：修改QP属性
>
> 返回：成功-返回0；失败-返回-1，并设置errno

#### QP操作
> - `int ibv_query_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr,int attr_mask,struct ibv_qp_init_attr *init_attr);`
>
> 描述：获取QP的属性以及初始化属性
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_query_srq(struct ibv_srq *srq, struct ibv_srq_attr *srq_attr);`
>
> 描述：获取SRQ属性
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `static inline int ibv_post_recv(struct ibv_qp *qp, struct ibv_recv_wr *wr,struct ibv_recv_wr **bad_wr)`
>
> 描述：将一个链表WRs发送到QP接收队列中，至少有一个接收队列被发送到QP中，并将QP的状态设置为RTR
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `static inline int ibv_post_send(struct ibv_qp *qp, struct ibv_send_wr *wr,struct ibv_send_wr **bad_wr)`
>
> 描述：将一个链表WRs发送到QP的发送队列中，
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `static inline int ibv_post_srq_recv(struct ibv_srq *srq, struct ibv_recv_wr *recv_wr,struct ibv_recv_wr **bad_recv_wr)`
>
> 描述：将一个WR链表发送到指定的SRQ中；
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `static inline int ibv_req_notify_cq(struct ibv_cq *cq, int solicited_only)`
>
> 描述：设置solicited_only标志的WRs将会被触发一个通知
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_get_cq_event(struct ibv_comp_channel *channel,struct ibv_cq **cq, void **cq_context);`
>
> 描述：获取完成队列事件
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `void ibv_ack_cq_events(struct ibv_cq *cq, unsigned int nevents);`
>
> 描述：确认从ibv_get_cq_event收到的事件

> - `static inline int ibv_poll_cq(struct ibv_cq *cq, int num_entries, struct ibv_wc *wc)`
>
> 描述：从完成队列中检索CQE
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_init_ah_from_wc(struct ibv_context *context, uint8_t port_num,struct ibv_wc *wc, struct ibv_grh *grh,struct ibv_ah_attr *ah_attr);`
>
> 描述：用wc中的信息初始化AH
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `struct ibv_ah *ibv_create_ah_from_wc(struct ibv_pd *pd, struct ibv_wc *wc,struct ibv_grh *grh, uint8_t port_num);`
>
> 描述：用wc的信息来创建AH，时将`ibv_init_ah_from_wc`和`ibv_create_ah`进行了结合
>
> 返回：成功-返回指向AH的指针，失败-返回-1

> - `int ibv_attach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid);`
>
> 描述：将QP加入到gid和lid指定的多播组中
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `int ibv_detach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid);`
>
> 描述：将QP从gid和lid指定的多播组中删除
>
> 返回：成功-返回0；失败-返回-1，并设置errno

#### 事件处理
> - `int ibv_get_async_event(struct ibv_context *context,struct ibv_async_event *event);`
>
> 描述：获取RDMA设备上下文的异步事件，是一个阻塞函数
>
> 返回：成功-返回0；失败-返回-1，并设置errno

> - `void ibv_ack_async_event(struct ibv_async_event *event);`
>
> 描述：必须使用该函数确认所有由`ibv_get_async_event`返回的事件

> - `const char *ibv_event_type_str(enum ibv_event_type event);`
>
> 描述：将事件类型转换为字符串



