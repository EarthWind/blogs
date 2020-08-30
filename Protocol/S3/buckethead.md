这个请求可以判断bucket是否存在一个请求用户是否有权限对bucket进行操作,如果bucket存在并且对bucket有权限，则返回200，如果bucket不存在，返回404，如果权限不允许，则返回403
## 请求参数
- Bucket: 必填，bucket名称

Method: HEAD

## 请求体
**无**

## 请求返回
成功：204(No Content)

头部可包含一下字段：
- x-amz-id-2: 服务器id
- x-amz-request-id: 请求id
- x-amz-bucket-region: bucket的region
- Date: 日期/Wed, 01 Mar  2006 12:00:00 GMT
- Connection: 底层tcp连接状态/close
- Server: 指定服务名称

## 参考链接
[DeleteBucket](https://docs.aws.amazon.com/AmazonS3/latest/API/API_DeleteBucket.html)