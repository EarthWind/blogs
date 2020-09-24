## AbortMultipartUpload
该请求会终止分片上传，在分片上传终止后，不会允许再有其他的分片使用该分片上传的id，之前上传分片使用的存储也会被释放，但是，如果在终止时正在上传的分片，状态是不确定的，可能成功，也可能不成功；所以，多次调用该请求来终止分片上传以释放其上传所使用的所有存储;
为了确保所有分片都已删除，可以使用ListPart请求确保分片上传以正确删除；

## 请求参数
- Bucket: 终止的分片上传所在的bucket
- Key: 终止的分片的对象key
- uploadId: 需要终止的分片上传id
- x-amz-expected-bucket-owner: 如果bucket的owner和该参数不一致，则403错误
- x-amz-request-payer: 确认该请求会被收费

## 返回体
- x-amz-request-charged: 该请求已被正常收费，可选值为requester

**返回码**204**

## 参考链接
- [AbortMultipartUpload](https://docs.aws.amazon.com/AmazonS3/latest/API/API_AbortMultipartUpload.html)
