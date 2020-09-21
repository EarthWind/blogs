## PutObject
上传一个对象到到bucket时，需要上传用户对bucket又写权限(WRITE)；上传的对象只会成功部分，当客户端收到服务端的放回时，如果没有出错，则代表整个对象的数据已经写入了存储集群中，如果返回失败，则不会保存对象的任何数据；由于对象存储是一个分布式的对象存储，所以当多个客户端同时上传相同的对象时(相同bucket下相同的key)，最后上传成功的对象会覆盖先上传的对象(对于没有版本的bucket而言)，如果需要保证对象不被覆盖，则需要在应用层进行相应的实现；在安全性上，为了保证数据在传输过程中不发生损坏或被篡改，可以使用HTTP头部字段Content-MD5进行数据校验，服务在处理带有该字段的请求时需要计算数据的MD5值，如果和该值不一致，则会返回客户端错误，保证了传数据数据的正确性，另一种方式是，在对象传输完成后由应用自己计算MD5值和服务端响应的ETag字段值进行比较，相等则代表数据上传无异常；

**服务端加密**

当请求中需要进行服务端加密时，对象在存入数据中心的设备之前会将数据进行加密，访问对象时数据会被再次解密并返回给客户端，用户可以提供加密密钥或则使用AWS管理的加密密钥来进行加密；

**对象ACL**

默认情况下，对象时私有的，只有所属者有所有的控制权限；在上传对象时，可以在请求中赋予其他用户或预定义组对象的权限；

**存储类**

默认清空下，存储类别使用的是STANDARD存储类，它能供提供高可用和高可靠性；如果存在性能上的需求，可以指定其他的存储类；

**对象版本**

如果bucket开启了版本功能，S3为自动给每个上传的对象生成唯一的版本ID，服务端会将这个ID返回给客户端，另外，如果多个客户端同时上传相同的对象(对象Key)，则所有的对象都会被保存而不是覆盖，并且每个对象有自己独立的版本ID；

## 对象名称
**安全字符**

| 类型 | 值 |
| --- | --- |
| 字母字符 | 0-9 |
| | a-z |
| | A-Z |
| 特殊字符 | 正斜杠 / |
| | 感叹号! |
| | 连字符 - |
| | 下划线 _ |
| | 句号 . |
| | 星号 * |
| | 单引号 ` |
| | 开括号 ( |
| | 关括号 ) |

**特殊处理字符**

| 类型 | 值 |
| --- | --- |
| | & |
| | $ |
| 不可打印字符 | 0-31(00-1F)和127(7F) |
| | @ |
| | = |
| | ; |
| | : |
| | + |
| | 空格 | 
| | , |
| | ? |

**避免字符**

| 类型 | 值 |
| --- | --- |
| | \ |
| | { |
| 不可打印字符 | 128-255 |
| | ^ |
| | } |
| | 引号 |
| | ` |
| | ] |
| | ~ |
| | < |
| | # |
| | \| |



## 请求参数
- Bucket: 必填，bucket名称
- Cache-Control: 用于指定请求和返回链路上的缓存特性([缓存头Cache-Control的含义和使用](https://juejin.im/post/6844903683046506504))
- Content-Disposition: 指定对象的描述信息([Hi gays, 你造Content-Disposition吗？](https://juejin.im/post/6844903911740932110))
- Content-Encoding: 指定内容编码格式([HTTP 协议中的 Content-Encoding](https://imququ.com/post/content-encoding-header-in-http.html))
- Content-Language: 内容语言([Content-Language](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Content-Language))
- Content-Length: 请求体的内容长度，单位为字节
- Content-MD5: 基于RFC1864 base64编码消息MD5摘要
- Content-Type: 内容数据格式的标准MIME类型([Content-Type](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Content-Type))
- Expires: 对象的缓存过期日期和时间
- Key: 对象名称
- x-amz-acl: 对象的内置acl权限
- x-amz-grant-full-control: 赋予全部权限
- x-amz-grant-read：被赋予读权限的用户能够获取对象数据和元数据
- x-amz-grant-read-acp：赋予能够读取对象的ACL
- x-amz-grant-write-acp：赋予能够修改对象的ACL
- x-amz-object-lock-legal-hold: 指定一个合法的锁定是否可以应用到该对象上，可选值：ON | OFF
- x-amz-object-lock-mode: 应用到该对象的对象锁模式，可选值：GOVERNANCE | COMPLIANCE
- x-amz-object-lock-retain-until-date: 该对象上对象锁的过期时间
- x-amz-request-payer: 请求者确定该请求会被收费，可选值：requester
- x-amz-server-side-encryption: 对象存储到后端存储时使用的加密算法，可选值：AES256 | aws:kms
- x-amz-server-side-encryption-aws-kms-key-id: 如果参数x-amz-server-side-encryption指定的值为aws:kms，则该参数用于指定aws kms中的应用到该对象的CMK id
- x-amz-server-side-encryption-context: 指定对象加密的KMS加密内容
- x-amz-server-side-encryption-customer-algorithm: 指定用用于加密对象的算法
- x-amz-server-side-encryption-customer-key: 指定用于加密数据的加密密钥
- x-amz-server-side-encryption-customer-key-MD5: 指定用于加密密钥的MD5校验值
- x-amz-storage-class: 指定对象的存储类，不指定则会选取默认的存储类，可选值为：STANDARD | REDUCED_REDUNDANCY | STANDARD_IA | ONEZONE_IA | INTELLIGENT_TIERING | GLACIER | DEEP_ARCHIVE
- x-amz-tagging: 对象的标签对，以URL查询参数的形式进行编码(key1=value1)
- x-amz-website-redirect-location: 如果bucket被配置为一个静态站点，该参数会将获取该对象的请求重定向到同bucket内对象或外部的链接

Method: PUT

## 请求体
对象内容

## 请求返回
成功：200

头部可包含一下字段：
- ETag: 对象的实体标签，一般为对象的MD5内容的MD5值
- x-amz-expiration：如果上传时包含了Expires字段，请求返回需要包含该字段，指定了对象的缓存过期时间
- x-amz-request-charged：如果存在，则表明请求者已成功为请求付费，可选值：requester
- x-amz-server-side-encryption：对应请求参数内容
- x-amz-server-side-encryption-aws-kms-key-id：对应请求参数内容
- x-amz-server-side-encryption-context：对应请求参数内容
- x-amz-server-side-encryption-customer-algorithm：对应请求参数内容
- x-amz-server-side-encryption-customer-key-MD5：对应请求参数内容
- x-amz-version-id：对象的版本id

## 参考链接
- [PutObject](https://docs.aws.amazon.com/AmazonS3/latest/API/API_PutObject.html)
- [Object key and metadata](https://docs.aws.amazon.com/AmazonS3/latest/dev/UsingMetadata.html)
