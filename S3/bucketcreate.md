## 请求参数
- Bucket: 必填，bucket名称
- x-amz-acl: 内置acl，包括private | public-read | public-read-write | authenticated-read
- x-amz-bucket-object-lock-enabled: 是否使能对象所
- x-amz-grant-full-control: 赋予指定用户该buket的所有控制权限
- x-amz-grant-read: 赋予指定用户的读权限
- x-amz-grant-read-acp: 赋予指定用户的读权限
- x-amz-grant-write: 赋予指定用户的写权限
- x-amz-grant-write-acp: 赋予指定用户的写权限

Method: PUT

## 请求体
可以指定bucket的区域(region)
```
<CreateBucketConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/"> 
    <LocationConstraint>Europe</LocationConstraint> 
</CreateBucketConfiguration >
```

## 请求返回
成功：200

头部可包含一下字段：
- x-amz-id-2: 服务器id
- x-amz-request-id: 请求id
- Date: 日期/Wed, 01 Mar  2006 12:00:00 GMT
- Location: 指定的bucket region
- Content-Length: 返回体长度
- Connection: 底层tcp连接状态
- Server: 指定服务名称

## bucket名称约束
- 3-63个字符
- 只能又小写字母、数字、句号(.)以及横杠(-)组成
- 必须以字母或数字开头
- 禁止写成IP地址的格式，比如192.168.5.4
- 虽然允许，单推荐不要使用句号作为bucket名称，特别是使用bucket存储静态资源的场景

## 参考链接
[CreateBucket](https://docs.aws.amazon.com/AmazonS3/latest/API/API_CreateBucket.html)