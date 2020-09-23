## CompleteMultipartUpload
通过将之前的上传的分片进行合并来完成一个上传；首先你需要初始化一个分片上传，然后调用UploadPart接口来上传所有的数据，当完成相关part的上传后，就可以调用该请求来完成一个upload，服务端收到该请求后，会将之前上传的part按照part number升序组织成一个对象；在CompeleteMultipartUpload请求中，需要提供所有上传的part列表，用于和服务端确认所有part已上传完成，该请求会将提供的part列表联系起来，对于每一个part，需要提供part number和上传part时服务端返回的ETAG值；
处理一个ComplteMultipartUpload可能需要花费几分钟的时间，在S3开始处理请求时，服务端会返回一个200的响应，在处理过程中，S3会周期性的发送空白字符以避免链接超时，因为有可能在收到200后该请求在后续的处理中出现了失败，检查返回体决定请求是否成功是很有必要的；
如果CompleteMultipartUpload请求失败了，那么客户端需要重试上传失败的part;
CompleteMultipartUpload请求有以下特殊的错误：
- EntityTooSmall: 400，除了最后一个part外，其他的part都需要大于5MB
- InvalidPart: 400，有部分part没有找到或则part的ETAG值不匹配
- InvalidPartOrder: 400，请求的part列表不是升序排序
- NoSuchUpload: 404，提供的uploadID不存在

## 请求参数
- Bucket: 必填，bucket名称；
- Key: 必填，对象名称
- uploadId: 必填，上传ID
- x-amz-expected-bucket-owner: 如果bucket所属者和该参数值不一致，返回403错误
- x-amz-request-payer: 客户端确认该请求会被收费

## 请求体
- CompleteMultipartUpload: XML顶层标签
- Part: 分片信息

```
<?xml version="1.0" encoding="UTF-8"?>
<CompleteMultipartUpload xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
   <Part>
      <ETag>string</ETag>
      <PartNumber>integer</PartNumber>
   </Part>
   ...
</CompleteMultipartUpload>
```

## 返回
- ETag: 对象etag
- x-amz-request-charged: 确认该请求已被正常收费
- x-amz-server-side-encryption: 服务端加密算法
- x-amz-server-side-encryption-aws-kms-key-id: 服务端加密使用的kms id
- x-amz-server-side-encryption-customer-algorithm: 客户端加密算法
- x-amz-server-side-encryption-customer-key-MD5: 客户端加密秘钥的md5值

## 参考链接
[CompleteMultipartUpload](https://docs.aws.amazon.com/AmazonS3/latest/API/API_UploadPart.html)

