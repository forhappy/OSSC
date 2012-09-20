# 项目简介 #
本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，取名为OSSC，
目前OSSC提供了OSS开放接口中所描述的所有功能, 特点包括：
	1. Bucket所有操作，如创建 bucket,删除，设置ACL，获取bucket的信息。
	2. Object所有操作，创建object（PUT），获取Object，拷贝，删除，删除多个，
		上传下载文件。
	3. Multipart Upload操作，初始化Multipart Upload，上传Part，终止Multipart Upload，
		查看Multipart Upload。
	4. Object Group操作，创建Object Group，获取Object Group，删除Object Group。

# 安装与体验 #
  ## 编译步骤 ##
      1. 安装CMake。
      2. 创建 build 目录，进入到该目录，执行 cmake ../.
      3. 编译和安装 make && make install 

# 项目特色 #
OSSC（OSS-C-SDK）完全采用C语言开发，并实现了类似面向对象的调用方式，
所有对象的“成员函数“采用函数指针形式实现，我们尽量和OSS-JAVA的使用方式切合，
每个对象都有相关的头文件和实现文件，并存放在modules目录下，
其中所有的的文件名均以oss_为前缀，每个文件实现了一个对象（或模块）。

# 简单示例 #

# 关于作者 #
傅海平：中国科学院计算技术研究所网络数据中心学生(haipingf@gmail.com)
王  维：中国科院学计算技术研究所网络数据中心学生(wangwei881116@gmail.com)
