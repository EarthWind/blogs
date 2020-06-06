# ENUM
> - ibv_port_state
```C
enum ibv_port_state {
        IBV_PORT_NOP            = 0,
        IBV_PORT_DOWN           = 1,
        IBV_PORT_INIT           = 2,
        IBV_PORT_ARMED          = 3,
        IBV_PORT_ACTIVE         = 4,
        IBV_PORT_ACTIVE_DEFER   = 5
};
```
> - ibv_mtu
```C
enum ibv_mtu {
        IBV_MTU_256  = 1,
        IBV_MTU_512  = 2,
        IBV_MTU_1024 = 3,
        IBV_MTU_2048 = 4,
        IBV_MTU_4096 = 5
};
```

> - ibv_device_cap_flags
```C
enum ibv_device_cap_flags {
        IBV_DEVICE_RESIZE_MAX_WR        = 1,
        IBV_DEVICE_BAD_PKEY_CNTR        = 1 <<  1,
        IBV_DEVICE_BAD_QKEY_CNTR        = 1 <<  2,
        IBV_DEVICE_RAW_MULTI            = 1 <<  3,
        IBV_DEVICE_AUTO_PATH_MIG        = 1 <<  4,
        IBV_DEVICE_CHANGE_PHY_PORT      = 1 <<  5,
        IBV_DEVICE_UD_AV_PORT_ENFORCE   = 1 <<  6,
        IBV_DEVICE_CURR_QP_STATE_MOD    = 1 <<  7,
        IBV_DEVICE_SHUTDOWN_PORT        = 1 <<  8,
        IBV_DEVICE_INIT_TYPE            = 1 <<  9,
        IBV_DEVICE_PORT_ACTIVE_EVENT    = 1 << 10,
        IBV_DEVICE_SYS_IMAGE_GUID       = 1 << 11,
        IBV_DEVICE_RC_RNR_NAK_GEN       = 1 << 12,
        IBV_DEVICE_SRQ_RESIZE           = 1 << 13,
        IBV_DEVICE_N_NOTIFY_CQ          = 1 << 14,
        IBV_DEVICE_MEM_WINDOW           = 1 << 17,
        IBV_DEVICE_UD_IP_CSUM           = 1 << 18,
        IBV_DEVICE_XRC                  = 1 << 20,
        IBV_DEVICE_MEM_MGT_EXTENSIONS   = 1 << 21,
        IBV_DEVICE_MEM_WINDOW_TYPE_2A   = 1 << 23,
        IBV_DEVICE_MEM_WINDOW_TYPE_2B   = 1 << 24,
        IBV_DEVICE_RC_IP_CSUM           = 1 << 25,
        IBV_DEVICE_RAW_IP_CSUM          = 1 << 26,
        IBV_DEVICE_MANAGED_FLOW_STEERING = 1 << 29
};
```

> - ibv_access_flags
```C
enum ibv_access_flags {
        IBV_ACCESS_LOCAL_WRITE          = 1,
        IBV_ACCESS_REMOTE_WRITE         = (1<<1),
        IBV_ACCESS_REMOTE_READ          = (1<<2),
        IBV_ACCESS_REMOTE_ATOMIC        = (1<<3),
        IBV_ACCESS_MW_BIND              = (1<<4),
        IBV_ACCESS_ZERO_BASED           = (1<<5),
        IBV_ACCESS_ON_DEMAND            = (1<<6),
};
```

> - ibv_qp_type
```C
enum ibv_qp_type {
        IBV_QPT_RC = 2,
        IBV_QPT_UC,
        IBV_QPT_UD,
        IBV_QPT_RAW_PACKET = 8,
        IBV_QPT_XRC_SEND = 9,
        IBV_QPT_XRC_RECV,
        IBV_QPT_DRIVER = 0xff,
};
```
> - ibv_qp_state
```C
enum ibv_qp_state {
        IBV_QPS_RESET,
        IBV_QPS_INIT,
        IBV_QPS_RTR,
        IBV_QPS_RTS,
        IBV_QPS_SQD,
        IBV_QPS_SQE,
        IBV_QPS_ERR,
        IBV_QPS_UNKNOWN
};
```

> - ibv_qp_attr_mask
```C
enum ibv_qp_attr_mask {
        IBV_QP_STATE                    = 1 <<  0,
        IBV_QP_CUR_STATE                = 1 <<  1,
        IBV_QP_EN_SQD_ASYNC_NOTIFY      = 1 <<  2,
        IBV_QP_ACCESS_FLAGS             = 1 <<  3,
        IBV_QP_PKEY_INDEX               = 1 <<  4,
        IBV_QP_PORT                     = 1 <<  5,
        IBV_QP_QKEY                     = 1 <<  6,
        IBV_QP_AV                       = 1 <<  7,
        IBV_QP_PATH_MTU                 = 1 <<  8,
        IBV_QP_TIMEOUT                  = 1 <<  9,
        IBV_QP_RETRY_CNT                = 1 << 10,
        IBV_QP_RNR_RETRY                = 1 << 11,
        IBV_QP_RQ_PSN                   = 1 << 12,
        IBV_QP_MAX_QP_RD_ATOMIC         = 1 << 13,
        IBV_QP_ALT_PATH                 = 1 << 14,
        IBV_QP_MIN_RNR_TIMER            = 1 << 15,
        IBV_QP_SQ_PSN                   = 1 << 16,
        IBV_QP_MAX_DEST_RD_ATOMIC       = 1 << 17,
        IBV_QP_PATH_MIG_STATE           = 1 << 18,
        IBV_QP_CAP                      = 1 << 19,
        IBV_QP_DEST_QPN                 = 1 << 20,
        /* These bits were supported on older kernels, but never exposed from
           libibverbs:
        _IBV_QP_SMAC                    = 1 << 21,
        _IBV_QP_ALT_SMAC                = 1 << 22,
        _IBV_QP_VID                     = 1 << 23,
        _IBV_QP_ALT_VID                 = 1 << 24,
        */
        IBV_QP_RATE_LIMIT               = 1 << 25,
};
```

> - ibv_srq_attr_mask
```C
enum ibv_srq_attr_mask {
        IBV_SRQ_MAX_WR  = 1 << 0,
        IBV_SRQ_LIMIT   = 1 << 1
};
```

> - ibv_wr_opcode
```C
enum ibv_wr_opcode {
        IBV_WR_RDMA_WRITE,
        IBV_WR_RDMA_WRITE_WITH_IMM,
        IBV_WR_SEND,
        IBV_WR_SEND_WITH_IMM,
        IBV_WR_RDMA_READ,
        IBV_WR_ATOMIC_CMP_AND_SWP,
        IBV_WR_ATOMIC_FETCH_AND_ADD,
        IBV_WR_LOCAL_INV,
        IBV_WR_BIND_MW,
        IBV_WR_SEND_WITH_INV,
        IBV_WR_TSO,
};
```

> - ibv_wc_status
```C
enum ibv_wc_status {
        IBV_WC_SUCCESS,
        IBV_WC_LOC_LEN_ERR,
        IBV_WC_LOC_QP_OP_ERR,
        IBV_WC_LOC_EEC_OP_ERR,
        IBV_WC_LOC_PROT_ERR,
        IBV_WC_WR_FLUSH_ERR,
        IBV_WC_MW_BIND_ERR,
        IBV_WC_BAD_RESP_ERR,
        IBV_WC_LOC_ACCESS_ERR,
        IBV_WC_REM_INV_REQ_ERR,
        IBV_WC_REM_ACCESS_ERR,
        IBV_WC_REM_OP_ERR,
        IBV_WC_RETRY_EXC_ERR,
        IBV_WC_RNR_RETRY_EXC_ERR,
        IBV_WC_LOC_RDD_VIOL_ERR,
        IBV_WC_REM_INV_RD_REQ_ERR,
        IBV_WC_REM_ABORT_ERR,
        IBV_WC_INV_EECN_ERR,
        IBV_WC_INV_EEC_STATE_ERR,
        IBV_WC_FATAL_ERR,
        IBV_WC_RESP_TIMEOUT_ERR,
        IBV_WC_GENERAL_ERR,
        IBV_WC_TM_ERR,
        IBV_WC_TM_RNDV_INCOMPLETE,
};
```

> - ibv_wc_opcode
```C
enum ibv_wc_opcode {
        IBV_WC_SEND,
        IBV_WC_RDMA_WRITE,
        IBV_WC_RDMA_READ,
        IBV_WC_COMP_SWAP,
        IBV_WC_FETCH_ADD,
        IBV_WC_BIND_MW,
        IBV_WC_LOCAL_INV,
        IBV_WC_TSO,
/*
 * Set value of IBV_WC_RECV so consumers can test if a completion is a
 * receive by testing (opcode & IBV_WC_RECV).
 */
        IBV_WC_RECV                     = 1 << 7,
        IBV_WC_RECV_RDMA_WITH_IMM,

        IBV_WC_TM_ADD,
        IBV_WC_TM_DEL,
        IBV_WC_TM_SYNC,
        IBV_WC_TM_RECV,
        IBV_WC_TM_NO_TAG,
};
```

> - LINK_LAYER
```C
enum {
        IBV_LINK_LAYER_UNSPECIFIED,
        IBV_LINK_LAYER_INFINIBAND,
        IBV_LINK_LAYER_ETHERNET,
};
```

> - ibv_send_flags
```C
enum ibv_send_flags {
        IBV_SEND_FENCE          = 1 << 0,
        IBV_SEND_SIGNALED       = 1 << 1,
        IBV_SEND_SOLICITED      = 1 << 2,
        IBV_SEND_INLINE         = 1 << 3,
        IBV_SEND_IP_CSUM        = 1 << 4
};
```

