## 请求参数
- Bucket: 必填，bucket名称

Method: DELETE

## 请求体
**无**

## 请求返回
成功：204(No Content)

头部可包含一下字段：
- x-amz-id-2: 服务器id
- x-amz-request-id: 请求id
- Date: 日期/Wed, 01 Mar  2006 12:00:00 GMT
- Connection: 底层tcp连接状态
- Server: 指定服务名称

## 参考链接
[DeleteBucket](https://docs.aws.amazon.com/AmazonS3/latest/API/API_DeleteBucket.html)
