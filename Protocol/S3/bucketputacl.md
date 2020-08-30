## PutBucketAcl
可以使用ACL来设置已存在bucket的权限，如果要修改bucket的权限，你必须对bucket有WRITE_ACP权限，否则无法修改，具有以下两种方式发送一个ACL请求
- 将ACL放在请求体中
- 将ACL放在请求头部

**注意**：以上的两种方式不能同时存在，选择哪一种方式取决你的应用的需求，

#### 访问权限
- 由头部x-amz-acl指定内置权限(Canned ACL)，和下一种方法不能同时使用
- 由头部x-amz-grant-read,x-amz-grant-read-acp,x-amz-grant-write-acp,x-amz-grant-full-control指定明确的权限，和上一种方法不能同时使用
- 在请求体中构建明确的权限，参考请求体部分的XML格式

## 请求参数
- Bucket: 指定修改ACL的bucket
- Content-MD5: base64编码的128位MD5校验值，用于检测请求的内容是否完整，在使用请求体设置权限时使用
- x-amz-acl: 设置内置权限，可选值private | public-read | public-read-write | authenticated-read
- x-amz-grant-full-control: 指定被被赋予所有控制权限的用户
- x-amz-grant-read: 指定被赋予读权限的用户
- x-amz-grant-read-acp: 指定被赋予能够读取bucket acl的用户
- x-amz-grant-write: 指定被赋予具有在bucket内创建、覆盖、删除object权限的用户
- x-amz-grant-write-acp: 指定被赋予修改bucket acl权限的用户

Method: PUT

## 请求体
将ACL放在请求体时存在请求体：
- AccessControlPolicy: xml顶层tag
- Grants: 赋予的权限信息(AccessControlList)
- > Grant: Grantee位被赋予权限的用户信息，Permission位赋予的权限，包括`FULL_CONTROL | WRITE | WRITE_ACP | READ | READ_ACP`
- Owner: bucket的所属者用户信息

```
<AccessControlPolicy xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
    <AccessControlList>
        <Grant>
            <Grantee>
                <DisplayName>string</DisplayName>
                <EmailAddress>string</EmailAddress>
                <ID>string</ID>
                <xsi:type>string</xsi:type>
                <URI>string</URI>
            </Grantee>
            <Permission>string</Permission>
        </Grant>
    </AccessControlList>
    <Owner>
        <DisplayName>string</DisplayName>
        <ID>string</ID>
    </Owner>
</AccessControlPolicy>
```

## 请求返回
成功：200

## 参考链接
[PutBucketAcl](https://docs.aws.amazon.com/AmazonS3/latest/API/API_PutBucketAcl.html)
