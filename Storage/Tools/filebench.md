# filebench
## 安装
```
#第一步
sudo yum -y install libtool automake byacc flex bison
git clone https://github.com/filebench/filebench.git
libtoolize
aclocal
autoheader
automake --add-missing
autoconf

#第二步
make
#make install
```

## 参考列表
> - [filesystem测试工具之filebench](http://www.yangguanjun.com/2017/07/08/fs-testtool-filebench/)


