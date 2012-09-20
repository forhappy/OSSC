/*
 * =============================================================================
 *
 *       Filename:  mainpage.h
 *
 *    Description:  mainpage content
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */


/** @mainpage OSSC 开发者手册
*
* @section OSSC介绍
* @subsection 关于OSS
* 阿里云存储服务(OpenStorageService,简称 OSS)，是阿里云对外提供的海量，安全，低成本，高可靠的云存储服务。
* 用户可以通过简单的 REST 接口，在任何时间、任何地点、任何互联网设备上进行上传和下载数据，
* 也可以使用WEB 页面对数据进行管理。同时，OSS 提供 Java、Python、PHP SDK,简化用户的编程。
* 基于 OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等基于大规模数据的服务。
* 【摘自《OSS API 开放接口规范文档》】。
*
* @subpage OSSC_INTRO
* @subsection 我们为OSS做了什么？
* 本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，并取名为 OSSC，
* 目前 OSSC 提供了 OSS 开放接口中所描述的所有功能, 特点包括：
* -# Bucket 所有操作，如创建 Bucket、删除 Bucket、获取某个 Bucket 访问权限、设置 Bucket 访问权限、获取所有 Bucket 信息、
* 获取 Bucket 中所有 Object 的信息。
* -# Object 所有操作，创建 Object（PUT），获取 Object，删除 Object，获取 Object 元信息，拷贝 Object，一次性删除多个 Object，另外，还在此基础上\n
* 实现了从文件上传 Object、从内存缓冲区上传 Object、下载 Object 至文件、下载 Object 至内存缓冲区、多线程断点续传上传大文件，断点续传下载文件。
* -# Multipart Upload 操作，初始化 Multipart Upload、上传 Part、完成 Multipart 上传、终止 Multipart Upload、查看 Multipart Upload，查看正在上传的 Part。
* -# Object Group 操作，创建 Object Group，获取 Object Group，获取 Object Group 中的 Object List 信息、获取 Object Group 元信息，删除 Object Group。
*
* @section OSSC安装细节
* @subpage OSSC_INSTALL
* @subsection OSSC依赖库
* OSSC 采用 CURL 库处理 HTTP 请求，因此在编译 OSSC 之前你需要安装 CURL，CURL 源码中包含了C调用API，最新版 CURL下载地址：http://curl.haxx.se/libcurl/。
* 除此之外，OSSC 不依赖任何其他程序库。
*
* @subsubsection CURL安装
* 本节介绍如何编译 CURL
*
* -# 下载 CURL，http://curl.haxx.se/download.html
* -# 安装 CURL，在Unix/Linux按照如下步骤，
* @code
* $ ./configure
* $ make
* $ make test (optional)
* $ make install(需root用户权限)
* @endcode
* 你也可以参考 CURL 的官方安装文档，http://curl.haxx.se/docs/install.html
* -# 完成依赖库安装后执行 ldconfig（需root用户权限）
*
* @subsection OSSC编译步骤
* 本节介绍如何编译OSSC。
*
* OSSC采用 CMake 管理构建过程，应该先安装CMake，
* -# 安装CMake。
* -# 创建 build 目录，进入到该目录，执行 cmake ../.
* -# 编译和安装 make && make install
*
* OSSC默认安装在 /usr/local目录下，当然你可以在 cmake 中设置。
*
* @subsection 在你的程序中使用OSSC
* OSSC以程序库的形式提供给上层开发者使用，因此如果你想基于OSSC开发上层应用，必须链接OSSC程序库，OSSC大部分都集中到了 osscore 中，\n
* 只是支持多线程调用模式的API单独放在 ossextra库中（由于时间有限，目前多线程只支持 pthread 线程库，后期会考虑在 Windows 下也支持多线程）\n
* 以下是你的程序需要连接OSSC，链接参数为：-L/path-to-your-ossc-installation -losscore.
*
* 另外需要注意的是，OSSC支持多线程断点续传模式下上传和下载文件，如果你想体验该功能，你需要链接如下库：
* -L/path-to-your-ossc-installation -lossextra.
*
* @section OSSC编码规范
* 一个优秀的项目必须遵循良好的编码规范，良好的编码风格可以促进团队协作，减少 BUG 产生几率，有助于开发后的代码审查，降低后期维护成本。
*
* OSSC虽然只是我们在业余时间完成的小项目，但是也注重的编码风格的一致性，我们强烈建议那些希望基于OSSC做二次开发的编码人员阅读本规范，
* 相信本规范能够让你快速熟悉 OSSC 的整体结构和OSSC API 的使用方法。
*
* @subpage OSSC_CODING_STYLE
*
* @section OSSC实现原理
*
* @subsection 概括
* OSSC（OSS-C-SDK）完全采用C语言开发，并实现了类似面向对象的调用方式，即“对象（struct 结构）”的“成员函数（函数指针）“采用函数指针形式实现，
* 我们尽量和OSS-JAVA的使用方式切合，每个对象都有相关的头文件和实现文件，并存放在modules目录下，其中与oss功能相关的所有的文件名均以oss_为前缀，\n
* 每个文件实现了一个对象（即模块），另外，为了方便开发人员记忆，OSSC中的所有结构和函数我们采用如下命名；
*
* -# 类名（本文指代struct 结构）以 “oss_” 为前缀，并以 “_t” 为后缀，名字中间的单词用来标识该结构的用途，单词以下划线“_”连接，比如：包含终止一个Multipart上传事件的请求参数被命名为 \n
* oss_abort_multipart_upload_request_t，或者包含访问阿里云开放存储服务的入口“类”被命名为 oss_client_t，其他以此类推。
* -# 与各个类（struct 结构指针，以下均以“类”指代）对应的函数均以函数指针的形式调用（除了oss_client_t），比如oss_abort_multipart_upload_request_t中设置 Bucket 名称可以按照如下方式调用，
* @code
* request->set_bucket_name(request，"bucket-name-001");
* @endcode
* -# 所有类对外均提供初始化函数和析构函数，其它与该类相关的操作均通过该类的函数指针调用，其中初始化函数命名规则为：该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+initialize，
* 析构函数为该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+finalize。
* -# 访问阿里云开放存储服务（Open Storage Service， OSS）的入口“类”是 oss_client_t，与此对应的所有函数均以client_前缀开头，并且第一个参数都是指向client结构的指针。
*
* 比如上传一个对象，你需要定义标识上传对象的请求结构（oss_put_object_request_t *request）和\n
* 上传对象之后返回信息的结构(oss_put_object_result_t *result)，然后将request作为参数传递给上传对象函数中（client_put_object_from_file(）\n
* 或者client_put_object()中），最后返回值会保存在result指针所指向的结构中。
*
* 下面的代码概括了使用 OSSC 的步骤(单线程上传文件示例)：
* @code
* const char *access_id = "ACSGmv9fkV1TDO9L";// Access ID
* const char *access_key = "BedoWbsje2"; // Access key
* onst char *bucket_name = "bucketname001"; //Bucket 名称
* const char *key = "put-test.data"; // Key, Object 名称
*
* FILE *file = fopen("proactor.pdf", "r"); //打开需要上传的文件
* unsigned short retcode = 0; //设置返回码，关于返回码的定义将在后文中详细介绍
* oss_client_t *client = client_initialize(access_id, access_key); // 初始化 oss_client_t *clien
*
* oss_object_metadata_t *metadata = object_metadata_initialize(); // 初始化 oss_object_metadata_t *metadata
* metadata->set_content_length(metadata, file_len); // 设置 metadata 元信息
* metadata->set_content_type(metadata, "application/octet-stream"); // 设置 metadata 元信息
*
* client_put_object_from_file(client, bucket_name, key, file, metadata, &retcode);// 核心操作：调用上传对象API
* client_finalize(client); //使用以后程序退出时需要释放 oss_client_t *client 分配的空间
*
* @endcode
*
* OSSC详细的实现原理请参考：\subpage OSSC_INTERNAL
*
* @section OSSC高级模块Extra库
* @subpage OSSC_EXTRA
* @section API使用示例
*
* @subpage OSSC_API_EXAMPLE
* @section 关于OSSC授权
* OSSC 采用 LGPL（GNU Lesser General Public License：GNU 宽通用公共许可证）授权形式发布，有关 LGPL 你可以查阅 GNU 官方文档：
* http://www.gnu.org/licenses/lgpl.html
* @section 关于作者
* 傅海平：中国科学院计算技术研究所网络数据中心学生(haipingf@gmail.com)\n
* 王  维：中国科院学计算技术研究所网络数据中心学生(wangwei881116@gmail.com)
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////
/** @page OSSC_INTRO OSSC介绍
 * @section OSSC介绍
 * @subsection OSS 简介
* 阿里云存储服务(OpenStorageService,简称 OSS)，是阿里云对外提供的海量，安全，低成本，高可靠的云存储服务。
* 用户可以通过简单的 REST 接口，在任何时间、任何地点、任何互联网设备上进行上传和下载数据，
* 也可以使用WEB 页面对数据进行管理。同时，OSS 提供 Java、Python、PHP SDK,简化用户的编程。
* 基于 OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等基于大规模数据的服务。
* 【摘自《OSS API 开放接口规范文档》】。
*
* @subsection OSSC概述
* 本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，并取名为 OSSC，
* 目前 OSSC 提供了 OSS 开放接口中所描述的所有功能, 特点包括：
* -# Bucket 所有操作，如创建 Bucket、删除 Bucket、获取某个 Bucket 访问权限、设置 Bucket 访问权限、获取所有 Bucket 信息、
* 获取 Bucket 中所有 Object 的信息。
* -# Object 所有操作，创建 Object（PUT），获取 Object，删除 Object，获取 Object 元信息，拷贝 Object，一次性删除多个 Object，另外，还在此基础上\n
* 实现了从文件上传 Object、从内存缓冲区上传 Object、下载 Object 至文件、下载 Object 至内存缓冲区、多线程断点续传上传大文件，断点续传下载文件。
* -# Multipart Upload 操作，初始化 Multipart Upload、上传 Part、完成 Multipart 上传、终止 Multipart Upload、查看 Multipart Upload，查看正在上传的 Part。
* -# Object Group 操作，创建 Object Group，获取 Object Group，获取 Object Group 中的 Object List 信息、获取 Object Group 元信息，删除 Object Group。
*
* @subsection 我们的夙愿
* 我们是 OSSC（OSS C SDK） 的开发者，OSSC 为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，目前 OSSC 相比 OSS JAVA可能还存在一些不足，并且程序稳定性有待检验，
* 但是在功能上完全实现了 OSS 开放接口，所以就功能而言，OSSC 已经达到在生产环境中使用的级别。
*
* 但是由于时间仓促，加上我们自身能力的有限，OSSC 不可能做得十分完美，我们将它开源贡献出来，希望今后更多的人参与到 OSSC 项目上来，提高 OSSC 的稳定性，希望借此促进阿里云
* 开放存储服务OSS的发展，同时我们也为开源贡献力量。
*
*/

/** @page OSSC_INSTALL OSSC安装步骤
* @section OSSC安装步骤
* @subsection OSSC依赖库
* OSSC 采用 CURL 库处理 HTTP 请求，因此在编译 OSSC 之前你需要安装 CURL，CURL 源码中包含了C调用API，最新版 CURL下载地址：http://curl.haxx.se/libcurl/。
* 除此之外，OSSC 不依赖任何其他程序库。
*
* @subsubsection CURL安装
* 本节介绍如何编译 CURL
*
* -# 下载 CURL，http://curl.haxx.se/download.html
* -# 安装 CURL，在Unix/Linux按照如下步骤，
* @code
* $ ./configure
* $ make
* $ make test (optional)
* $ make install(需root用户权限)
* @endcode
* 你也可以参考 CURL 的官方安装文档，http://curl.haxx.se/docs/install.html
* -# 完成依赖库安装后执行 ldconfig（需root用户权限）
*
* @subsection OSSC编译步骤
* 本节介绍如何编译OSSC。
*
* OSSC采用 CMake 管理构建过程，应该先安装CMake，
* -# 安装CMake。
* -# 创建 build 目录，进入到该目录，执行 cmake ../.
* -# 编译和安装 make && make install
*
* OSSC默认安装在 /usr/local目录下，当然你可以在 cmake 中设置。
*
* @subsection 在你的程序中使用OSSC
* OSSC以程序库的形式提供给上层开发者使用，因此如果你想基于OSSC开发上层应用，必须链接OSSC程序库，OSSC大部分都集中到了 osscore 中，\n
* 只是支持多线程调用模式的API单独放在 ossextra库中（由于时间有限，目前多线程只支持 pthread 线程库，后期会考虑在 Windows 下也支持多线程）\n
* 以下是你的程序需要连接OSSC，链接参数为：-L/path-to-your-ossc-installation -losscore.
*
* 另外需要注意的是，OSSC支持多线程断点续传模式下上传和下载文件，如果你想体验该功能，你需要链接如下库：
* -L/path-to-your-ossc-installation -lossextra.
 */


/** @page OSSC_INTERNAL OSSC实现原理
 *
 * @section 概述
 * OSSC（OSS-C-SDK）完全采用C语言开发，所有代码（头文件 + 实现文件 + 测试文件）合计约 2.2 万行，我们在 OSSC 中实现了类似面向对象的调用方式，\n
 * 因此熟悉 OSS Java调用方式的开发者可以在很短的时间内掌握 OSS C SDK的使用。
 * @section 代码目录结构
 * @code
 * .
 * ├── AUTHORS
 * ├── CMakeLists.txt
 * ├── config.h.cmake.in
 * ├── doc
 * │   └── doxygen
 * ├── include
 * │   ├── ossc
 * │   └── ossc-config.h
 * ├── LICENSE.txt
 * ├── README.txt
 * ├── src
 * │   ├── CMakeLists.txt
 * │   ├── core
 * │   ├── lib
 * │   ├── modules
 * │   └── util
 * ├── test
 *
 * @endcode
 *
 * OSSC 采用 CMake 管理项目构建过程，源码主要存放在 src 中，头文件在
 */




/** @page OSSC_CODING_STYLE OSSC编码规范
* -# 类名（本文指代struct 结构）以 “oss_” 为前缀，并以 “_t” 为后缀，名字中间的单词用来标识该结构的用途，单词以下划线“_”连接，比如：包含终止一个Multipart上传事件的请求参数被命名为 \n
* oss_abort_multipart_upload_request_t，或者包含访问阿里云开放存储服务的入口“类”被命名为 oss_client_t，其他以此类推。
* -# 与各个类（struct 结构指针，以下均以“类”指代）对应的函数均以函数指针的形式调用（除了oss_client_t），比如oss_abort_multipart_upload_request_t中设置 Bucket 名称可以按照如下方式调用，
* @code
* request->set_bucket_name(request，"bucket-name-001");
* @endcode
* -# 所有类对外均提供初始化函数和析构函数，其它与该类相关的操作均通过该类的函数指针调用，其中初始化函数命名规则为：该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+initialize，
* 析构函数为该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+finalize。
* -# 访问阿里云开放存储服务（Open Storage Service， OSS）的入口“类”是 oss_client_t，与此对应的所有函数均以client_前缀开头，并且第一个参数都是指向client结构的指针。
*
 */

/** @page OSSC_EXTRA OSSC高级模块Extra库
* @section OSSC高级模块Extra简介
* OSSC 高级模块中包含了多线程上传下载文件的 API，由于时间和精力有限，我们目前并没有实现 Windows 平台的多线程上传下载功能，希望今后会有其他开发者实现
* 这一功能。
*
* OSSC 采用了POSIX多线程标准库 pthread,理论上只要你的操作系统支持 pthread都可以使用 OSSC 的 extra 库中的 API。
*
* @section OSSC_EXTRA_API_INTRO OSSC Extra API 介绍
* OSSC Extra API 提供了 2 个有关多线程上传下载的 API，它们分别是：
* @code
* client_extra_put_object();
* client_extra_get_object();
* @endcode
* @section OSSC_EXTRA_API_USAGE OSSC Extra API使用
*/


/** @page OSSC_API_EXAMPLE OSSC API 使用示例
* @section Bucket操作
* @subsection 创建Bucket
* @code
* client_create_bucket(client,bucket_name,&retcode);
* @endcode
* @subsection 获取Bucket的ACL信息
* @subsection 设置Bucket的ACL
* @subsection 列举Bucket下的Object
* @subsection 获取所有的Bucket
* @subsection 删除Bucket
*
* @section Object操作
* @subsection 上传Object
* @subsection 获取Object
* @subsection 拷贝Object
* @subsection 获取Object元信息
* @subsection 删除Object
* @subsection 一次性删除多个Object
*
* @section Multipart Upload操作
* @subsection Initiate Multipart Upload 操作
* @subsection Upload Part 上传
* @subsection Complete Multipart Upload 操作
* @subsection Abort Multipart Upload 操作
* @subsection List Multipart Uploads 操作
* @subsection List Parts 操作
*
* @section Object Group 操作
* @subsection Post Object Group 操作
* @subsection Get Object Group 上传
* @subsection Complete Multipart Upload 操作
* @subsection Get Object Group Index 操作
* @subsection Head Object Group 操作
* @subsection Delete Object Group 操作
*/


