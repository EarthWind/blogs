## HeadObject
HEAD操作会检索对象元数据，但是不会返回对象内容，如果只关心对象元数据，就可以使用该请求，注意HEAD操作需要对对象有读权限；HEAD和GET具有想用得请求参数，唯一得不同是在响应中没有对象内容得返回体，如果在上传对象时使用了客户端提供得密钥进行加密，则在请求中需要提供下面得请求头字段：
- x-amz-server-side-encryption-customer-algorithm
- x-amz-server-side-encryption-customer-key
- x-amz-server-side-encryption-customer-key-MD5
注意，请求头会限制在8KB以内；

**权限**
HEAD操作需要对象得s3:GetObject权限，如果没有该权限，返回得错误码由是否有s3:ListBucket权限而不同；
- 有s3:ListBucket权限，则返回404(no such key)
- 没有s3:ListBucket权限，则返回403(access dennied)
