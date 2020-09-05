## GetObject
从S3中检索对象，为了能够获取数据，必须有对象的读权限，如果对象给匿名用户赋予了读权限，则可以不适用认证就可以获取对象内容；对象没有像文件系统一样的层级结构，但是你可以使用对象的key来进行模拟，例如，有个对象的key位sample.jpg，你可以把它命名为photos/2006/February/sample.jpg，在获取对象时，需要在头部写权这个逻辑上的层级结构；
对于读取频率高的大对象，可以使用BitTorrent来节省带宽。对于存储在GLACIER和DEEP_ARCHIVE存储类中的数据，在检索之前必须调用RestoreObject恢复对象，否则服务端将会返回InvalidObjectStateError报错。

#### 权限
必须有s3:GetObject权限才能够获取对象，如果对象不存在，返回的错误取决于是否有s3:ListBucket权限
- 有s3:ListBucket权限，则返回404 ("no such key") 
- 否则返回403 ("access denied")

#### 版本
默认获取当前版本的对象，需要获取其他对象时，则在头部指定对应的versionId

#### 覆盖返回头


## 请求参数
- Bucket: 必填，bucket名称
- Key: 必填，需要获取的对象名称
- If-Match: 如果指定的ETag和对象一致，返回对象，否则返回412
- If-Modified-Since：返回指定时间后修改的对象，否则返回304(not modified)
- If-None-Match: 如果指定的ETag和对象不一致，返回对象，否则返回304(not modified)
- If-Unmodified-Since: 如果指定时间后没有修改，返回对象，否则返回 412 (precondition failed).
- partNumber: 指定part名称
- Range: 指定获取数据范围
- response-cache-control: 设置响应的Cache-Control返回头
- response-content-disposition: 设置响应的Content-Disposition返回头
- response-content-encoding: 设置响应的Content-Encoding返回头
- response-content-language: 设置响应的Content-Language返回头
- response-content-type: 设置响应的Content-Type返回头
- response-expires: 设置响应的Expires返回头
- versionId: 指定要返回的数据版本
- x-amz-request-payer: 确认该请求会被收费
- x-amz-server-side-encryption-customer-algorithm: 服务端加密算法
- x-amz-server-side-encryption-customer-key: 指定客户端提供给服务端进行数据加密的key
- x-amz-server-side-encryption-customer-key-MD5: 指定128为通过RFC 1321加密的MD5值

Method: GET

## 请求体
**无**

## 请求返回
成功：200
- accept-ranges: 返回指定的range值
- Cache-Control: 请求要求返回的值
- Content-Disposition: 请求要求返回的值
- Content-Encoding: 请求要求返回的值
- Content-Language: 请求要求返回的值
- Content-Length: 请求要求返回的值
- Content-Range: 请求要求返回的值
- Content-Type: 对象数据类型
- ETag: 资源表示
- Expires: 对象的缓存过期时间
- Last-Modified: 对象最近修改的时间
- x-amz-delete-marker：指定对象是否是标记删除
- x-amz-expiration
- x-amz-missing-meta
- x-amz-mp-parts-count
- x-amz-object-lock-legal-hold
- x-amz-object-lock-mode
- x-amz-object-lock-retain-until-date
- x-amz-replication-status
- x-amz-request-charged
- x-amz-restore
- x-amz-server-side-encryption
- x-amz-server-side-encryption-aws-kms-key-id
- x-amz-server-side-encryption-customer-algorithm
- x-amz-server-side-encryption-customer-key-MD5
- x-amz-storage-class
- x-amz-tagging-count
- x-amz-version-id
- x-amz-website-redirect-location