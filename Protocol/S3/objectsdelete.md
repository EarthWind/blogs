## DeleteObjects
DeleteObjects使用户可以在一个HTTP请求中删除多个对象，这样可以减少请求的开销；请求中用XML格式组织了不超过1000个要删除对象的信息，对象名称，如果在带用版本的bucket中可以指定要删除的对象版本，每个删除对象都会有相应的返回，时独立的，不管成功或失败，即在返回中可能包含删除成功的对象，也包含删除未成功的对象；注意，如果删除的对象不存在，则在请求返回时显示成功；
该请求支持两种模式的响应：
- 简洁模式(quiet)：响应包含了出现错误的对象，如果对象全部删除成功，则响应不包含任何返回体；
- 详细模式(verbose)：包含每个对象删除情况，成功或失败原因
默认情况下为详细模式，最后，该请求中必须包含Content-MD5头，保证删除的对象在传输过程中不被修改；

## 请求参数
- Bucket: 必填，要删除对象的bucket
- x-amz-bypass-governance-retention: 指定是否绕过对象的治理锁
- x-amz-mfa: 
- x-amz-request-payer: 客户端明确该请求会被收费；

Method: DELETE

## 请求体
- Delete：必填，顶层XML
- Object: 对象删除列表
- Quiet：是否使用简介模式
```
<Delete xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
   <Object>
      <Key>string</Key>
      <VersionId>string</VersionId>
   </Object>
   ...
   <Quiet>boolean</Quiet>
</Delete>
```

## 请求返回
成功：200 OK
- x-amz-request-charged: 请求已被成功收费
- DeleteResult: 必填，删除结果，顶层XML
- Deleted: 包含成功删除的对象
- Error: 包含删除时发送错误的对象

```
<DeleteResult>
   <Deleted>
      <DeleteMarker>boolean</DeleteMarker>
      <DeleteMarkerVersionId>string</DeleteMarkerVersionId>
      <Key>string</Key>
      <VersionId>string</VersionId>
   </Deleted>
   ...
   <Error>
      <Code>string</Code>
      <Key>string</Key>
      <Message>string</Message>
      <VersionId>string</VersionId>
   </Error>
   ...
</DeleteResult>
```
