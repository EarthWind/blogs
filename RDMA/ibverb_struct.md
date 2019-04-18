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
        __be64                  node_guid;              //节点guid
        __be64                  sys_image_guid;         
        uint64_t                max_mr_size;            
        uint64_t                page_size_cap;
        uint32_t                vendor_id;              //
        uint32_t                vendor_part_id;
        uint32_t                hw_ver;
        int                     max_qp;                 //最大的queue pair队列
        int                     max_qp_wr;              //每个queue pair的最多请求
        unsigned int            device_cap_flags;
        int                     max_sge;
        int                     max_sge_rd;
        int                     max_cq;                 //最多由多少个完成队列
        int                     max_cqe;                //每个完成队列最多由多少个元素
        int                     max_mr;
        int                     max_pd;
        int                     max_qp_rd_atom;
        int                     max_ee_rd_atom;
        int                     max_res_rd_atom;
        int                     max_qp_init_rd_atom;
        int                     max_ee_init_rd_atom;
        enum ibv_atomic_cap     atomic_cap;
        int                     max_ee;
        int                     max_rdd;
        int                     max_mw;
        int                     max_raw_ipv6_qp;
        int                     max_raw_ethy_qp;
        int                     max_mcast_grp;
        int                     max_mcast_qp_attach;
        int                     max_total_mcast_qp_attach;
        int                     max_ah;
        int                     max_fmr;
        int                     max_map_per_fmr;
        int                     max_srq;
        int                     max_srq_wr;
        int                     max_srq_sge;
        uint16_t                max_pkeys;
        uint8_t                 local_ca_ack_delay;
        uint8_t                 phys_port_cnt;
};
```
