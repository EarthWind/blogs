## CreateMultipartUpload
multipartUpload API可以分part上传大对象，CreateMultipartUpload初始化一个multipartUpload并返回一个上传ID，这个ID会见指定得multipart中所有得parts关联起来，在每一个上传part(UploadPart)的请求中需要带上这个ID，另外在完成以及终止multipart上传的请求中都需要带上；如果bucket配置了终止multipart上床的生命周期规则，则上传的需要在指定的时间内完成，否则上传请求会在到期后被终止掉；对于请求的认证情况，时一个一系列常规的请求，初始化multipart,上传part,结束上传过程，每个请求都时独立认证的，和其他请求没有相比没有特殊的地方；

## 请求参数
- Bucket: 上传的bucket名称
- Cache-Control: 上传和下载链上的缓存特性
- Content-Disposition: 对象内容描述
- Content-Encoding: 内容编码
- Content-Language: 内容语言
- Content-Type:资源类型
- Expires: 过期时间
- Key: 对象key
- x-amz-acl: 对象权限
- x-amz-grant-full-control: 赋予全控制权限的用户列表
- x-amz-grant-read: 赋予读权限的用户列表
- x-amz-grant-read-acp: 赋予能供读取对象权限的用户列表
- x-amz-grant-write-acp: 赋予能供修改对象权限的用户列表
- x-amz-object-lock-legal-hold: 是否锁定对象，ON | OFF
- x-amz-object-lock-mode: 指定对象的对象锁GOVERNANCE | COMPLIANCE
- x-amz-object-lock-retain-until-date: 指定对象所的过期时间
- x-amz-request-payer: 确认该请求会被收费
- x-amz-server-side-encryption: 服务端加密AES256 | aws:kms
- x-amz-server-side-encryption-aws-kms-key-id: 指定加密密钥的id
- x-amz-server-side-encryption-context: 指定用于对象加密的加密上下文
- x-amz-server-side-encryption-customer-algorithm: 指定用于加密对象的算法，比如AES256
- x-amz-server-side-encryption-customer-key: 指定用于加密的密钥
- x-amz-server-side-encryption-customer-key-MD5: 指定密钥的128位MD5校验值
- x-amz-storage-class: 存储类，默认为STANDARD, STANDARD | REDUCED_REDUNDANCY | STANDARD_IA | ONEZONE_IA | INTELLIGENT_TIERING | GLACIER | DEEP_ARCHIVE
- x-amz-tagging: 对象的标签集合
- x-amz-website-redirect-location: 对象的重定向地址

Method: PUT

## 请求返回
成功：200

- x-amz-abort-date: 如果bucket配置了生命周期规则，则在请求返回时会带上该字段
- x-amz-abort-rule-id: 和x-amz-abort-date一起返回，指定了生效的生命周期规则ID
- x-amz-request-charged: 确认该请求已收费
- x-amz-server-side-encryption: 服务端加密算法
- x-amz-server-side-encryption-aws-kms-key-id: 服务端加密使用的密钥ID
- x-amz-server-side-encryption-context: 服务端加密上下文
- x-amz-server-side-encryption-customer-algorithm: 客户端提供的服务端加密密钥
- x-amz-server-side-encryption-customer-key-MD5: 加密密钥的128位MD5校验值
- InitiateMultipartUploadResult: 顶层XML
> - Bucket: bucket名称
> - Key: 上传对象名称
> - UploadId: 上传ID

```
<InitiateMultipartUploadResult xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
    <Bucket>example-bucket</Bucket>
    <Key>example-object</Key>
    <UploadId>VXBsb2FkIElEIGZvciA2aWWpbmcncyBteS1tb3ZpZS5tMnRzIHVwbG9hZA</UploadId>
</InitiateMultipartUploadResult>
```

## 参考链接
- [CreateMultipartUpload](https://docs.aws.amazon.com/AmazonS3/latest/API/API_CreateMultipartUpload.html)