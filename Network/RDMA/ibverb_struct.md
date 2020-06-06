# STRUCT
> - ibv_device
```C
struct ibv_device {
    struct _ibv_device_ops  _ops;
    enum ibv_node_type      node_type;
    enum ibv_transport_type transport_type;
    /* Name of underlying kernel IB device, eg "mthca0" */
    char                    name[IBV_SYSFS_NAME_MAX];
    /* Name of uverbs device, eg "uverbs0" */
    char                    dev_name[IBV_SYSFS_NAME_MAX];
    /* Path to infiniband_verbs class device in sysfs */
    char                    dev_path[IBV_SYSFS_PATH_MAX];
    /* Path to infiniband class device in sysfs */
    char                    ibdev_path[IBV_SYSFS_PATH_MAX];
};
```
> - ibv_device_attr
```C
struct ibv_device_attr {
        char                    fw_ver[64];             //固件版本
        __be64                  node_guid;              //节点唯一标识符guid
        __be64                  sys_image_guid;         //系统镜像GUID
        uint64_t                max_mr_size;            //可注册的最大连续块
        uint64_t                page_size_cap;          //支持的页大小
        uint32_t                vendor_id;              //供应商IEEE ID
        uint32_t                vendor_part_id;         //供应商零件ID
        uint32_t                hw_ver;                 //硬件版本
        int                     max_qp;                 //最大的queue pair队列
        int                     max_qp_wr;              //每个queue pair的最多请求
        unsigned int            device_cap_flags;       //设备功能标志
        int                     max_sge;                //对于non-RD队列每个WR的最大scatter/gather数量
        int                     max_sge_rd;             //对于RD队列每个WR的最大scatter/gather数量
        int                     max_cq;                 //最多由多少个完成队列
        int                     max_cqe;                //每个完成队列最多由多少个元素
        int                     max_mr;                 //最大支持的内存区域
        int                     max_pd;                 //最大支持的保护域
        int                     max_qp_rd_atom;         //每个QP的read和原子操作的最大数量
        int                     max_ee_rd_atom;         //每个端到端链接RD最大read和原子操作数量
        int                     max_res_rd_atom;        //对于读和原子操作可以使用的最大资源
        int                     max_qp_init_rd_atom;    //
        int                     max_ee_init_rd_atom;
        enum ibv_atomic_cap     atomic_cap;             //原子功能
        int                     max_ee;                 //最大支持的端到端的上下文的数量
        int                     max_rdd;                //最多支持RD域的数量
        int                     max_mw;                 //MW的最大值
        int                     max_raw_ipv6_qp;        //最大支持的原始IPv6数据报的队列数量
        int                     max_raw_ethy_qp;        //最大支持的以太网数据报队列数量
        int                     max_mcast_grp;          //最多支持的多播组数量
        int                     max_mcast_qp_attach;    //每个多播组最多可以包含的队列数量
        int                     max_total_mcast_qp_attach;
                                                        //所有多播组总共可以包含的队列数量
        int                     max_ah;                 //最多支持的地址处理(Address Handle)
        int                     max_fmr;                //最多支持的快速内存区域(FMR:Fast Memory Region)的大小
        int                     max_map_per_fmr;        //
        int                     max_srq;                //最多支持的共享接收队列
        int                     max_srq_wr;             //每个共享接收队列中WR的最大值
        int                     max_srq_sge;            //每个共享队列最多的SGEs数量
        uint16_t                max_pkeys;              //
        uint8_t                 local_ca_ack_delay;     //
        uint8_t                 phys_port_cnt;          //物理端口数量
};
```

> - ibv_context
```C
struct ibv_context {
        struct ibv_device      *device;
        struct ibv_context_ops  ops;
        int                     cmd_fd;
        int                     async_fd;
        int                     num_comp_vectors;
        pthread_mutex_t         mutex;
        void                   *abi_compat;
};
```

> - ibv_port_attr
```C
struct ibv_port_attr {
        enum ibv_port_state     state;
        enum ibv_mtu            max_mtu;
        enum ibv_mtu            active_mtu;
        int                     gid_tbl_len;
        uint32_t                port_cap_flags;
        uint32_t                max_msg_sz;
        uint32_t                bad_pkey_cntr;
        uint32_t                qkey_viol_cntr;
        uint16_t                pkey_tbl_len;
        uint16_t                lid;
        uint16_t                sm_lid;
        uint8_t                 lmc;
        uint8_t                 max_vl_num;
        uint8_t                 sm_sl;
        uint8_t                 subnet_timeout;
        uint8_t                 init_type_reply;
        uint8_t                 active_width;
        uint8_t                 active_speed;
        uint8_t                 phys_state;
        uint8_t                 link_layer;
        uint8_t                 reserved;
};
```

> - ibv_qp_cap
```C
struct ibv_qp_cap {
        uint32_t                max_send_wr;    //发送队列中的发送请求的最大数量
        uint32_t                max_recv_wr;    //接收队列中的最大接收值
        uint32_t                max_send_sge;   //在一个发送请求中WR最多的scatter/gather数量
        uint32_t                max_recv_sge;   //在一个接收请求中WR最多的scatter/gather数量
        uint32_t                max_inline_data;//发送队列中的最大字节数量
};
```

> - ibv_pd
```C
struct ibv_pd {
        struct ibv_context     *context;
        uint32_t                handle;
};
```

> - ibv_cq
```C
struct ibv_cq {
        struct ibv_context     *context;
        struct ibv_comp_channel *channel;
        void                   *cq_context;
        uint32_t                handle;
        int                     cqe;

        pthread_mutex_t         mutex;
        pthread_cond_t          cond;
        uint32_t                comp_events_completed;
        uint32_t                async_events_completed;
};
```

> - ibv_comp_channel
```C
struct ibv_comp_channel {
        struct ibv_context     *context;
        int                     fd;
        int                     refcnt;
};
```

> - ibv_mr
```C
struct ibv_mr {
        struct ibv_context     *context;
        struct ibv_pd          *pd;
        void                   *addr;
        size_t                  length;
        uint32_t                handle;
        uint32_t                lkey;
        uint32_t                rkey;
};
```

> - ibv_qp
```C
struct ibv_qp {
        struct ibv_context     *context;
        void                   *qp_context;
        struct ibv_pd          *pd;
        struct ibv_cq          *send_cq;
        struct ibv_cq          *recv_cq;
        struct ibv_srq         *srq;
        uint32_t                handle;
        uint32_t                qp_num;
        enum ibv_qp_state       state;
        enum ibv_qp_type        qp_type;

        pthread_mutex_t         mutex;
        pthread_cond_t          cond;
        uint32_t                events_completed;
};
```

> - ibv_qp_attr
```C
struct ibv_qp_attr {
        enum ibv_qp_state       qp_state;
        enum ibv_qp_state       cur_qp_state;
        enum ibv_mtu            path_mtu;
        enum ibv_mig_state      path_mig_state;
        uint32_t                qkey;
        uint32_t                rq_psn;
        uint32_t                sq_psn;
        uint32_t                dest_qp_num;
        unsigned int            qp_access_flags;
        struct ibv_qp_cap       cap;
        struct ibv_ah_attr      ah_attr;
        struct ibv_ah_attr      alt_ah_attr;
        uint16_t                pkey_index;
        uint16_t                alt_pkey_index;
        uint8_t                 en_sqd_async_notify;
        uint8_t                 sq_draining;
        uint8_t                 max_rd_atomic;
        uint8_t                 max_dest_rd_atomic;
        uint8_t                 min_rnr_timer;
        uint8_t                 port_num;
        uint8_t                 timeout;
        uint8_t                 retry_cnt;
        uint8_t                 rnr_retry;
        uint8_t                 alt_port_num;
        uint8_t                 alt_timeout;
        uint32_t                rate_limit;
};
```

> - ibv_qp_init_attr
```C
struct ibv_qp_init_attr {
        void                   *qp_context;     //用户自定义的QP
        struct ibv_cq          *send_cq;        //发送队列
        struct ibv_cq          *recv_cq;        //接收队列
        struct ibv_srq         *srq;            //共享接收队列，仅在SRQ队列对中使用
        struct ibv_qp_cap       cap;            //指定QP的功能属性
        enum ibv_qp_type        qp_type;        //指定QP的类型
        int                     sq_sig_all;     //当该标志位为1时，所有的发送请求(WR)都将会产生一个CQE，当该元素为0时，只有被`ibv_post_send`标记的WR才产生CQE
}
```
> - ibv_srq
```C
struct ibv_srq {
        struct ibv_context     *context;        //ibv_open_device返回的上下文结构
        void                   *srq_context;    //
        struct ibv_pd          *pd;             //保护域
        uint32_t                handle;

        pthread_mutex_t         mutex;
        pthread_cond_t          cond;
        uint32_t                events_completed;
};
```

> - ibv_srq_init_attr
```C
struct ibv_srq_init_attr {
        void                   *srq_context;    //
        struct ibv_srq_attr     attr;
};
```

> - ibv_srq_attr
```C
struct ibv_srq_attr {
        uint32_t                max_wr;         //SRQ队列中WR的最大值
        uint32_t                max_sge;        //每个WR的scatter最大数量
        uint32_t                srq_limit;      //SRQ的限制值
};
```

> - ibv_ah_attr
```C
struct ibv_ah_attr {
        struct ibv_global_route grh;
        uint16_t                dlid;
        uint8_t                 sl;
        uint8_t                 src_path_bits;
        uint8_t                 static_rate;
        uint8_t                 is_global;
        uint8_t                 port_num;
};
```

> - ibv_ah_attr
```C
struct ibv_ah_attr {
        struct ibv_global_route grh;
        uint16_t                dlid;
        uint8_t                 sl;
        uint8_t                 src_path_bits;
        uint8_t                 static_rate;
        uint8_t                 is_global;
        uint8_t                 port_num;
};
```

> - ibv_recv_wr
```C
struct ibv_recv_wr {
        uint64_t                wr_id;
        struct ibv_recv_wr     *next;
        struct ibv_sge         *sg_list;
        int                     num_sge;
};
```

> - ibv_sge
```C
struct ibv_sge {
        uint64_t                addr;           //缓冲区地址
        uint32_t                length;         //缓冲区长度
        uint32_t                lkey;           //调用ibv_reg_mr生成的local key
};
```

> - ibv_send_wr
```C
struct ibv_send_wr {
        uint64_t                wr_id;
        struct ibv_send_wr     *next;
        struct ibv_sge         *sg_list;
        int                     num_sge;
        enum ibv_wr_opcode      opcode;
        unsigned int            send_flags;
        /* When opcode is *_WITH_IMM: Immediate data in network byte order.
         * When opcode is *_INV: Stores the rkey to invalidate
         */
        union {
                __be32                  imm_data;
                uint32_t                invalidate_rkey;
        };
        union {
                struct {
                        uint64_t        remote_addr;
                        uint32_t        rkey;
                } rdma;
                struct {
                        uint64_t        remote_addr;
                        uint64_t        compare_add;
                        uint64_t        swap;
                        uint32_t        rkey;
                } atomic;
                struct {
                        struct ibv_ah  *ah;
                        uint32_t        remote_qpn;
                        uint32_t        remote_qkey;
                } ud;
        } wr;
        union {
                struct {
                        uint32_t    remote_srqn;
                } xrc;
        } qp_type;
        union {
                struct {
                        struct ibv_mw   *mw;
                        uint32_t                rkey;
                        struct ibv_mw_bind_info bind_info;
                } bind_mw;
                struct {
                        void                   *hdr;
                        uint16_t                hdr_sz;
                        uint16_t                mss;
                } tso;
        };
};
```

> - ibv_wc
```C
struct ibv_wc {
        uint64_t                wr_id;
        enum ibv_wc_status      status;
        enum ibv_wc_opcode      opcode;
        uint32_t                vendor_err;
        uint32_t                byte_len;
        /* When (wc_flags & IBV_WC_WITH_IMM): Immediate data in network byte order.
         * When (wc_flags & IBV_WC_WITH_INV): Stores the invalidated rkey.
         */
        union {
                __be32          imm_data;
                uint32_t        invalidated_rkey;
        };
        uint32_t                qp_num;
        uint32_t                src_qp;
        unsigned int            wc_flags;
        uint16_t                pkey_index;
        uint16_t                slid;
        uint8_t                 sl;
        uint8_t                 dlid_path_bits;
};
```

> - ibv_gid
```C
union ibv_gid {
        uint8_t                 raw[16];
        struct {
                __be64  subnet_prefix;
                __be64  interface_id;
        } global;
};
```
