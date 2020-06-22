# RDMA

## 1. What’s RDMA:
Remote Direct Memory Access (RDMA) is a technology that allows computers in a network to exchange data in main memory without involving the processor, cache or operating system of either computer.
- Remote: transfer data in a network.
- Direct: no Operating System Kernel involvement and everything offload onto interface card.
- Memory: user space application virtual memory, virtual memory.
- Access: send, receive, read(only in OFA), write(only in OFA) and atomics (only in IB) operations.

#### 1.1 Message passing
- a common programming model
- useful for communicating between any two process
- a message is a abstract chunk of data
- often used in parallel computing and interprocess communication
- processes send and receive messages to each other
- processes can use message passing to synchronize, by waiting for a message

> **RDMA is a way of transporting messages between servers or between a server and a client. RDMA is a message passing paradigm.**

## 2. Benefits
- Zero-copy : applications can perform data transfer without the network software stack involvement and data is being send received directly to the buffers without being copied between the network layers. data moves directly from user memory on one side to user memory on the other side.
- Kernel bypass : applications can perform data transfer directly from user space without the need to perform context switches. User has direct access to the Channel Adapter.
- No CPU involvement : applications can access remote memory without consuming any CPU in the remote machine. The remote memory machine will be read without any intervention of remote process (or processor). The caches in the remote CPU(s) won't be filled with the accessed memory content.
- Scatter/gather entries support : RDMA supports natively working with multiple scatter/gather entries i.e. reading multiple memory buffers and sending them as one stream or getting one stream and writing it to multiple memory buffers
- Message based transactions : the data is handled as discrete messages and not as a stream, which eliminates the need of the application to separate the stream into different messages/transactions.
- asynchronous operation : threads not blocked during I/O transfers

## 3. Goals of RDMA
- High Bandwidth Utilization
- Low Latency
- Low CPU utilization

## 4. RDMA Technologies Overview
there are several network protocols which support RDMA :
- RoCE: RDMA over Converged Ethernet. RoCE is a network protocol that enables RDMA over an Ethernet network by defining how it will perform in such an environment. It requires a very low CPU overhead and takes advantage of Priority Flow Control in Data Center Bridging Ethernet for lossless connectivity.
- iWARP: Internet Wide Area RDMA Protocol. IWARP leverages the Transmission Control Protocol (TCP) or Stream Control Transmission Protocol (SCTP) to transmit data.
- InfiniBand: RDMA is the standard protocol for high-speed InfiniBand network connections.

> All three technologies share a common user API, but have different physical and link layers.

- VPI: Mellanox Virtual Protocol Interconnect which provides high performance, low latency a reliable means for communication among network adapters and switches supporting both InfiniBand and Ethernet semantics.

## 5. message passing:
- SEND/RECEIVE : ‘channel semantic’, double ended operation.it involves both the Requester and the Responder. never uses rkey field in struct ibv_mr and never uses access rights IBV_ACCESS_REMOTE_READ or IBV_ACCESS_REMOTE_WRITE.
> - sender must issue listen() before client issues connect()
> - both sender and receiver must actively participate in all data transfer()
> - sender doesn’t know remote receiver’s virtual memory location
> - receiver doesn’t know remote sender’s virtual memory location

>***!NOTE***: 
> - order and timing of send() and recv() are relevant. memory involved in transfer is untouchable between start and completion of transfer.
> - Between Posting and Completion user memory containing message data is undefined and should NOT be changed by user program.

- RDMA READ/RDMA WRITE : ‘memory semantic’
> - active side calls rdma_post_write()
> - prior to issuing this operation, active side must obtain passive side’s address and key
> - passive side provides  “metadata” that enables the data “push”, but doesn’t participate in it.
> - never use rkey filed in struct ibv_mr
> - never use access rights IBV_ACCESS_REMOTE_READ or IBV_ACCESS_REMOTE_WRITE
!NOTE: on iWARP the client must be the size to send the first message.

## 6. QueuePair Type(Transport Modes)
- RC(Reliable Connection) QP: RC QP guaranteed that messages are delivered from a requester to responder at most once, in order and without corruption; Queue Pair is associated with only one other QP.
> - Send operations
> - Write operations
> - Read operations
> - Atomic operations

- UC(Unreliable Connection) QP: There isn't any guaranteed that the messages will be received by the other side: corrupted or out of sequence packets are silently dropped. If a packet is being dropped, the whole message that it belongs to will be dropped. In this case, the responder won't stop, but continues to receive incoming packets. There isn't any guarantee about the packet ordering.
> - Send operations
> - Write operations

- UD(Unreliable Datagram) QP: One QP can send and receive message to any other UD QP in either unicast (one to one) or multicast (one to many) way in an unreliable way. There isn't any guaranteed that the messages will be received by the other side: corrupted or out of sequence packets are silently dropped. There isn't any guarantee about the packet ordering.
> - Send operations

| Operation | UD | UC | RC | RD(not supported) |
| --- | --- | --- | --- |--- |
| Send(with immediate) | X | X | X | X |
| Receive | X | X | X | X |
| Write | | X | X | X |
| Read | | | X | X |
| Atomic | | | X | X |
| Max size | MTU | 1GB | 1GB | 1GB | 

> QP STATE
> - RESET   : Newly created, queue empty
> - INIT    : Basic information set. Ready for posting to receive queue
> - RTR     : Ready to Receive. Remote address info set for connected QPs. QP may now receive packets.
> - RTS     : Ready to Send. Timeout and retry parameters set. QP may now send packets
>
> These transitions are accomplished through the use of the `ibv_modify_qp` command.

## 8. Interpretation
- SR: Send Request
- RR: Receive Request
- CQ: Completion Queue. A completion queue holds completion queue entries(CQE).Each Queue Pair has an associated send and receive CQ.A single CQ can be shared for sending and receiving as well as be shared accross multiple QPs.
- MR: Memory Registration
- MW: Memory Window, The operation of associating an MW with an MR is called Binding. Different MWs can overlap the same MR(event with different access permissions);
- PD: Protection Domain, is used to associate Queue Pairs with Memory Regions and Memory Windows and limit which memory regions can be accessed by which queue pairs providing a degree of protection from unauthorized access.
- AE: Asynchronous Events, the network adapter may send async events to inform the SW about events that occurred in the system.
- AV: Address Vector, which is an object that describes the route from the local node to the remote node. struct ibv_ah is used to implement address vectors.
- SRQ: Shared receive queues
- GRH: Global Routing Header is used for routing between subnets.

## 9. Typical Application
- Get the device list
> - `struct ibv_device **ibv_get_device_list(int *num_devices);`

- Open the requested device
> - `struct ibv_context *ibv_open_device(struct ibv_device *device);`

- Query the device capabilities
- Allocate a Protection Domain to contain your resources
> - `struct ibv_pd *ibv_alloc_pd(struct ibv_context *context);`

- Register a memory region
> - `void *memalign (size_t __alignment, size_t __size);`
> - `struct ibv_mr *ibv_reg_mr(struct ibv_pd *pd, void *addr,size_t length, int access);`
> - 

- Create a Completion Queue(CQ)
> - `struct ibv_cq *ibv_create_cq(struct ibv_context *context, int cqe, void *cq_context,struct ibv_comp_channel *channel, int comp_vector);`

- Create a Queue Pair(QP)
> Creating a QP will also create an associated send queue and receive queue
```C
struct ibv_qp_init_attr init_attr = {
    .send_cq = send_queue,
    .recv_cq = recv_queue,
    .cap     = {
            .max_send_wr  = 1,
            .max_recv_wr  = rx_depth,
            .max_send_sge = 1,
            .max_recv_sge = 1
    },
    .qp_type = IBV_QPT_RC
};
```
> - `struct ibv_qp *ibv_create_qp(struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr);`
```C
struct ibv_qp_attr attr = {
    .qp_state        = IBV_QPS_INIT,
    .pkey_index      = 0,
    .port_num        = port,
    .qp_access_flags = 0
};
```
> - `ibv_modify_qp(ctx->qp, &attr,IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_ACCESS_FLAGS))`

- Bring up a QP
```C
struct pingpong_dest {
    int lid;            //First local identifier (LID) assigned to this port 
    int qpn;            //QP number
    int psn;            //lrand48() & 0xffffff;
    union ibv_gid gid;  //inet_ntop(AF_INET6, &my_dest.gid, gid, sizeof gid);
};
//transform these infomation with socket
```
```C
struct ibv_qp_attr attr = {
    .qp_state               = IBV_QPS_RTR,
    .path_mtu               = mtu,
    .dest_qp_num            = dest->qpn,
    .rq_psn                 = dest->psn,
    .max_dest_rd_atomic     = 1,
    .min_rnr_timer          = 12,
    .ah_attr                = {
            .is_global      = 0,
            .dlid           = dest->lid,
            .sl             = sl,
            .src_path_bits  = 0,
            .port_num       = port
    }
};

ibv_modify_qp(ctx->qp, &attr,
                IBV_QP_STATE              |
                IBV_QP_AV                 |
                IBV_QP_PATH_MTU           |
                IBV_QP_DEST_QPN           |
                IBV_QP_RQ_PSN             |
                IBV_QP_MAX_DEST_RD_ATOMIC |
                IBV_QP_MIN_RNR_TIMER)
```

> A created QP still can't be used until it is transitioned through several states, eventually getting to Ready To Send(RTS).
```C
attr.qp_state       = IBV_QPS_RTS;
attr.timeout        = 14;
attr.retry_cnt      = 7;
attr.rnr_retry      = 7;
attr.sq_psn         = my_psn;
attr.max_rd_atomic  = 1;

ibv_modify_qp(ctx->qp, &attr,
                IBV_QP_STATE              |
                IBV_QP_TIMEOUT            |
                IBV_QP_RETRY_CNT          |
                IBV_QP_RNR_RETRY          |
                IBV_QP_SQ_PSN             |
                IBV_QP_MAX_QP_RD_ATOMIC)
```
- Post work requests and poll for completion
> - `static inline int ibv_poll_cq(struct ibv_cq *cq, int num_entries, struct ibv_wc *wc)`

- Cleanup
> - Delete QP
> - Delete CQ
> - Deregister MR
> - Deallocate PD
> - Close device

## Reference
- [Which Queue Pair type to use?](https://www.rdmamojo.com/2013/06/01/which-queue-pair-type-to-use/)
- [RDMA Aware Networks Programming User Manual]()

