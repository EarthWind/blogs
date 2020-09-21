## UploadPart
上传multipart upload中一个part，在上传任何part之前，都需要先初始化mutipart upload，完成后服务端会在响应中获取一个独立唯一得ID，在上传part得时候需要带上这个ID，用于标识这个part属于那个upload;
Part Number可以是从1到10000(包括)的任意数字，一个PartNumber唯一的标识了一个part并确定了part在对象中的位置，PartNumber小的在前，PartNumber大的在对象的后面，如果上传了和之前的part相同的PartNumber，则之前的part将会被覆盖，除了上传的最后一个part没有大小限制之外，其他的part的大小都必须大于5M；
上传part时可以带上服务端加密的参数，这样数据在被写入磁盘之前会根据提供的加密秘密对数据进行加密，访问数据的时候会进行解密后返回给客户端；

**注意**
在初始化完成一个multipart upload并上传完成一些part后，这个upload要么调用complete或则调用abort，如果不调用，则用于上传part的存储会被单独收费；

## 请求参数
- Bucket: 必填，bucket名称
- Content-Length: 请求体的字节数，如果请求体的长度不能自动决定时，该参数是很有用的
- Content-MD5: part数据base64编码的128位MD5校验值
- Key: 必填，对象名称
- partNumber: 必填，正在上传的part的编号，该值是在1到10000之间的正整数
- uploadId: 必填，表示该part所属的upload
- x-amz-expected-bucket-owner: bucket的owner，如果bucket的owner和该值不一致，则会返回403错误(Access Denied)
- x-amz-request-payer: 确认该请求会被收费，有效值为requester
- x-amz-server-side-encryption-customer-algorithm: 制定加密对象的算法
- x-amz-server-side-encryption-customer-key: 指定客户端提供的加密对象的加密秘钥
- x-amz-server-side-encryption-customer-key-MD5: 加密秘钥的128位MD5值，基于RFC 1321标准

## 请求体
- Body

## 请求返回
- ETag: part的MD5校验值
- x-amz-request-charged: 该请求已被正常收费，有效值为requester
- x-amz-server-side-encryption: 服务端加密算法，可选值为AES256 | aws:kms
- x-amz-server-side-encryption-aws-kms-key-id: 服务端加密使用的kms id
- x-amz-server-side-encryption-customer-algorithm: 客户端指定服务端加密算法
- x-amz-server-side-encryption-customer-key-MD5: 服务端加密算法使用的加密秘钥的MD5值

## 参考链接
- [UploadPart](https://docs.aws.amazon.com/AmazonS3/latest/API/API_UploadPart.html)
