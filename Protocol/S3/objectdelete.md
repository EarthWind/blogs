## DeleteObject
删除一个没有版本的对象并在对象上添加删除标记，这使得对象会成为最近的带版本对象，如果没有不带版本的对象，则该操作不会删除任何对象；
只有bucket的所有者才能删除某个版本的对象，并且必须在请求中带有版本参数，这样会永久删除对象；如果指定删除的版本上有删除标记，则服务器会在删除请求响应中将x-amz-delete-marker设置为true；
除了直接调用删除接口删除对象外，还可以启用bucket生命周期(PutBucketLifecycle)，到达生命周期的对象会被自动删除，如果不允许删除bucket内的对象，需要取消其他用户的s3:DeleteObject,s3:DeleteObjectVersion以及s3:PutLifeCycleConfiguration的权限；

## 请求参数
- Bucket: 必填，bucket名称
- Key: 必填，要删除的对象名称
- versionId: 指定要删除的对象版本
- x-amz-bypass-governance-retention: 指定该操作是否绕过治理模式
- x-amz-mfa:
- x-amz-request-payer: 表示客户端知道该请求会比收费

Method: DELETE

## 请求体
**无**

## 请求返回
成功：204(No Content)

头部可包含一下字段：
- x-amz-delete-marker: 如果标记删除，则为true,否则为false
- x-amz-request-charged: 表明该请求以计费成功
- x-amz-version-id: 返回指定的删除对象版本或则生成的删除标记的对象版本
- x-amz-id-2: 服务器id
- x-amz-request-id: 请求id
- Date: 日期/Wed, 01 Mar  2006 12:00:00 GMT
- Connection: 底层tcp连接状态/close
- Server: 指定服务名称
