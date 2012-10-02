#########################################################
# 注意：强烈建议用户首先阅读 doc/html 目录中的相关页面，#
# 获取更多有关 OSSC 编码规范，实现原理和 API 示例等内容 #
#########################################################

# 项目简介 #
本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，取名为OSSC，
目前OSSC提供了OSS开放接口中所描述的所有功能, 基本特点包括：
	1. Bucket所有操作，如创建 bucket,删除，设置ACL，获取bucket的信息。
	2. Object所有操作，创建object（PUT），获取Object，拷贝，删除，删除多个，
		上传下载文件。
	3. Multipart Upload操作，初始化Multipart Upload，上传Part，终止Multipart Upload，
		查看Multipart Upload。
	4. Object Group操作，创建Object Group，获取Object Group，删除Object Group。

# 更多文档请参考 doc/html目录中的页面。#

# 安装与体验 #
  ## 编译步骤 ##
      1. 安装CMake。
      2. 创建 build 目录，进入到该目录，执行 cmake ../.
      3. 编译和安装 make && make install 

# 关于作者 #
傅海平：中国科学院计算技术研究所(haipingf@gmail.com)
王  维：中国科院学计算技术研究所(wangwei881116@gmail.com)
