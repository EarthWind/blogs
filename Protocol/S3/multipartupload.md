## UploadPart
上传multipart uplad中一个part，在上传任何part之前，都需要先初始化mutipart upload，完成后服务端会在响应中获取一个独立唯一得ID，在上传part得时候需要带上这个ID，用于标识这个part属于那个upload;
Part Number可以是从1到10000(包括)的任意数字，一个PartNumber唯一的标识了一个part并确定了part在对象中的位置，PartNumber小的在前，PartNumber大的在对象的后面，如果上传了和之前的part相同的PartNumber，则之前的part将会被覆盖，除了上传的最后一个part没有大小限制之外，其他的part的大小都必须大于5M；
上传part时可以带上服务端加密的参数，这样数据在被写入磁盘之前会根据提供的加密秘密对数据进行加密，访问数据的时候会进行解密后返回给客户端；

## 请求参数
- Bucket
- Content-Length
- Content-MD5
- Key
- partNumber
- uploadId
- x-amz-expected-bucket-owner
- x-amz-request-payer
- x-amz-server-side-encryption-customer-algorithm
- x-amz-server-side-encryption-customer-key
- x-amz-server-side-encryption-customer-key-MD5

## 请求体
- Body

## 请求返回
- ETag
- x-amz-request-charged
- x-amz-server-side-encryption
- x-amz-server-side-encryption-aws-kms-key-id
- x-amz-server-side-encryption-customer-algorithm
- x-amz-server-side-encryption-customer-key-MD5

## 参考链接
- [UploadPart](https://docs.aws.amazon.com/AmazonS3/latest/API/API_UploadPart.html)
