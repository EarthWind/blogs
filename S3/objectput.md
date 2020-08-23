上传一个对象到到bucket时，需要上传用户对bucket又写权限(WRITE)；上传的对象只会成功部分，当客户端收到服务端的放回时，如果没有出错，则代表整个对象的数据已经写入了存储集群中，如果返回失败，则不会保存对象的任何数据；由于对象存储是一个分布式的对象存储，所以当多个客户端同时上传相同的对象时，最后上传成功的对象会覆盖先前的对象，如果需要保证对象不被覆盖，则需要在应用层进行相应的实现；为了保证数据在传输过程中不发生损坏或被篡改，可以使用HTTP头部字段Content-MD5进行校验，对象存储在处理带有该字段的请求时计算数据的MD5值，如果和该值不一致，则会返回客户端错误，保证了传数据数据的正确性，另外，也可以在对象传输完成后由应用自己计算MD5值和服务端返回的ETAG字段值进行比较，相等则代表数据上传无异常；

**服务端加密**

当请求中需要进行服务端加密时，对象在存入数据中心的设备之前会被加密，访问对象时对象数据会被再次解密并返回给客户端

**对象ACL**

默认情况下，对象时私有的，只有所属者有所有的控制权限；在上传对象时，可以在请求中赋予其他用户对象的权限；

**存储类**

默认清空下，存储类别使用的是STANDARD存储类，它能供提供高可用和高可靠性；如果由性能上的需求，可以指定其他的存储类

**对象版本**

如果bucket开启了版本功能，S3为自动给存储的对象生成一个唯一的版本ID，服务端会将这个ID返回给客户端，另外，如果多个客户端同时上床相同的对象(对象名称)，则所有的对象都会被保存而不是覆盖，并且每个对象由自己独立的版本ID；

## 请求参数
- Bucket: 必填，bucket名称
- Cache-Control: 用于指定请求和放回链路上的缓存特性
- Content-Disposition: 指定对象的陈述信息
- Content-Encoding: 指定内容编码格式
- Content-Language: 内容语言
- Content-Length: 请求体的内容长度，单位为字节
- Content-MD5: 基于RFC1864 base64编码消息MD5摘要
- Content-Type: 内容数据格式的标准MIME类型
- Expires: 对象的过期日期和时间
- Key: 对象名称
- x-amz-acl: 对象的内置acl权限
- x-amz-grant-full-control: 赋予全部权限
- x-amz-grant-read：赋予读权限
- x-amz-grant-read-acp：赋予读权限
- x-amz-grant-write-acp：赋予写权限
- x-amz-object-lock-legal-hold: 
- x-amz-object-lock-mode: 
- x-amz-object-lock-retain-until-date: 
- x-amz-request-payer:
- x-amz-server-side-encryption:
- x-amz-server-side-encryption-aws-kms-key-id:
- x-amz-server-side-encryption-context:
- x-amz-server-side-encryption-customer-algorithm: 
- x-amz-server-side-encryption-customer-key:
- x-amz-server-side-encryption-customer-key-MD5:
- x-amz-storage-class:
- x-amz-tagging:
- x-amz-website-redirect-location:

Method: PUT

## 请求体
对象内容

## 请求返回
成功：200

头部可包含一下字段：
- ETag: 对象的实体标签
- x-amz-expiration：如果上传时包含了Expires字段，请求返回需要包含该字段
- x-amz-request-charged：
- x-amz-server-side-encryption：
- x-amz-server-side-encryption-aws-kms-key-id：
- x-amz-server-side-encryption-context：
- x-amz-server-side-encryption-customer-algorithm：
- x-amz-server-side-encryption-customer-key-MD5：
- x-amz-version-id：

## 参考链接
[PutObject](https://docs.aws.amazon.com/AmazonS3/latest/API/API_PutObject.html)