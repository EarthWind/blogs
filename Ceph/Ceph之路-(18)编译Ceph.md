# 开发者模式
## 准备环境
```SHELL
$ git clone -b v14.2.1 https://github.com/ceph/ceph.git
$ cd ceph/
$ git submodule update --init --recursive
$ sudo ./install-deps.sh
$ sudo yum -y install python-sphinx rdma-core libibverbs libibverbs-devel libudev libudev-devel e2fsprogs e2fsprogs-devel libblkid-devel openldap openldap-devel CUnit-devel libaio-devel leveldb-devel snappy-devel lz4-devel keyutils-libs-devel libcurl-devel nss-devel openssl-devel expat-devel liboath-devel python-devel lttng-ust-devel libbabeltrace-devel Cython gperf librabbitmq-devel
```
#### 修改编译文件
```SHELL
$ vim do_cmake.sh
#${CMAKE} -DCMAKE_BUILD_TYPE=Debug $ARGS "$@" .. || exit 1
$(CMAKE) -DCMAKE_C_FLAGS="-O0 -g3 -GDWARF-4" -DCMAKE_CXX_FLAGS="-O0 -g3 -gdwarf-4" -DBOOST_J=$(nproc) $ARGS "$@" .. || exit 1
```

#### 编译
```SHELL
$ cd build/
$ make -j 32
```

## 参考列表
- [深入理解crush(2)—-手动编译ceph集群并使用librados读写文件](https://www.dovefi.com/post/%E6%B7%B1%E5%85%A5%E7%90%86%E8%A7%A3crush2%E6%89%8B%E5%8A%A8%E7%BC%96%E8%AF%91ceph%E9%9B%86%E7%BE%A4%E5%B9%B6%E4%BD%BF%E7%94%A8librados%E8%AF%BB%E5%86%99%E6%96%87%E4%BB%B6/)


