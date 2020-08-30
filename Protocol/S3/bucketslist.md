返回属于请求用户的所有bucket
## 请求参数
无任何请求参数

Method: GET

## 请求体
**无**

## 请求返回
成功：200

```
<ListAllMyBucketsResult>
   <Buckets>
      <Bucket>
         <CreationDate>timestamp</CreationDate>
         <Name>string</Name>
      </Bucket>
   </Buckets>
   <Owner>
      <DisplayName>string</DisplayName>
      <ID>string</ID>
   </Owner>
</ListAllMyBucketsResult>
```

- Buckets: bucket信息列表，包含了每个bucket的创建时间和名称
- Owner: 包含用户信息

## 参考链接
[ListBuckets](https://docs.aws.amazon.com/AmazonS3/latest/API/API_ListBuckets.html)