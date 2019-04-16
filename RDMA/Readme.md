# RDMA

## 1. What’s RDMA:
> Remote Direct Memory Access (RDMA) is a technology that allows computers in a network to exchange data in main memory without involving the processor, cache or operating system of either computer.
> - Remote: transfer data in a network.
> - Direct: no Operating System Kernel involvement and everything offload onto interface card.
> - Memory: user space application virtual memory, virtual memory.
> - Access: send, receive, read(only in OFA), write(only in OFA) and atomics (only in IB) operations.

#### 1.1 Message passing
> - a common programming model
> - useful for communicating between any two process
> - a message is a abstract chunk of data
> - often used in parallel computing and interprocess communication
> - processes send and receive messages to each other
> - processes can use message passing to synchronize, by waiting for a message
>
> **RDMA is a way of transporting messages between servers or between a server and a client. RDMA is a message passing paradigm.**

## 2. Benefits
> - Zero-copy : applications can perform data transfer without the network software stack involvement and data is being send received directly to the buffers without being copied between the network layers. data moves directly from user memory on one side to user memory on the other side.
> - Kernel bypass : applications can perform data transfer directly from user space without the need to perform context switches. User has direct access to the Channel Adapter.
> - No CPU involvement : applications can access remote memory without consuming any CPU in the remote machine. The remote memory machine will be read without any intervention of remote process (or processor). The caches in the remote CPU(s) won't be filled with the accessed memory content.
> - Scatter/gather entries support : RDMA supports natively working with multiple scatter/gather entries i.e. reading multiple memory buffers and sending them as one stream or getting one stream and writing it to multiple memory buffers
> - Message based transactions : the data is handled as discrete messages and not as a stream, which eliminates the need of the application to separate the stream into different messages/transactions.
> - asynchronous operation : threads not blocked during I/O transfers

## 3. Goals of RDMA
> - High Bandwidth Utilization
> - Low Latency
> - Low CPU utilization

## 4. RDMA Technologies
there are several network protocols which support RDMA :
> - RoCE: RDMA over Converged Ethernet. RoCE is a network protocol that enables RDMA over an Ethernet network by defining how it will perform in such an environment.
> - iWARP: Internet Wide Area RDMA Protocol. IWARP leverages the Transmission Control Protocol (TCP) or Stream Control Transmission Protocol (SCTP) to transmit data.
> - InfiniBand: RDMA is the standard protocol for high-speed InfiniBand network connections.

## 5. message passing:
> - SEND/RECEIVE : ‘channel semantic’, double ended operation.it involves both the Requester and the Responder. never uses rkey field in struct ibv_mr and never uses access rights IBV_ACCESS_REMOTE_READ or IBV_ACCESS_REMOTE_WRITE.
>> - sender must issue listen() before client issues connect()
>> - both sender and receiver must actively participate in all data transfer()
>> - sender doesn’t know remote receiver’s virtual memory location
>> - receiver doesn’t know remote sender’s virtual memory location
>
>***!NOTE***: 
>> - order and timing of send() and recv() are relevant. memory involved in transfer is untouchable between start and completion of transfer.
>> - Between Posting and Completion user memory containing message data is undefined and should NOT be changed by user program.

> - RDMA READ/RDMA WRITE : ‘memory semantic’
>> - active side calls rdma_post_write()
>> - prior to issuing this operation, active side must obtain passive side’s address and key
>> - passive side provides  “metadata” that enables the data “push”, but doesn’t participate in it.
>> - never use rkey filed in struct ibv_mr
>> - never use access rights IBV_ACCESS_REMOTE_READ or IBV_ACCESS_REMOTE_WRITE
!NOTE: on iWARP the client must be the size to send the first message.





