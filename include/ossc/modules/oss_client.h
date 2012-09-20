/*
 * =============================================================================
 *
 *       Filename:  oss_client.h
 *
 *    Description:  oss client.
 *
 *        Created:  09/03/2012 09:16:21 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_CLIENT_H
# error Never include <ossc/modules/oss_client.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_CLIENT_H
#define OSS_CLIENT_H
#include "ossc-config.h"

#define _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
#include <ossc/modules/oss_abort_multipart_upload_request.h>
#undef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_ACCESS_CONTROL_LIST_H
#include <ossc/modules/oss_access_control_list.h>
#undef _oss_access_control_list_h

#define _OSS_BUCKET_H
#include <ossc/modules/oss_bucket.h>
#undef _OSS_BUCKET_H

#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H

#define _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#include <ossc/modules/oss_complete_multipart_upload_request.h>
#undef _oss_complete_multipart_upload_request_h

#define _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include <ossc/modules/oss_complete_multipart_upload_result.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H

#define _OSS_COPY_OBJECT_REQUEST_H
#include <ossc/modules/oss_copy_object_request.h>
#undef _OSS_COPY_OBJECT_REQUEST_H

#define _OSS_COPY_OBJECT_RESULT_H
#include <ossc/modules/oss_copy_object_result.h>
#undef _OSS_COPY_OBJECT_RESULT_H

#define _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
#include <ossc/modules/oss_delete_multiple_object_request.h>
#undef _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H

#define _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include <ossc/modules/oss_generate_presigned_url_request.h>
#undef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H

#define _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#include <ossc/modules/oss_get_object_group_index_result.h>
#undef _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H

#define _OSS_GET_OBJECT_GROUP_REQUEST_H
#include <ossc/modules/oss_get_object_group_request.h>
#undef _OSS_GET_OBJECT_GROUP_REQUEST_H

#define _OSS_GET_OBJECT_REQUEST_H
#include <ossc/modules/oss_get_object_request.h>
#undef _OSS_GET_OBJECT_REQUEST_H

#define _OSS_GRANT_H
#include <ossc/modules/oss_grant.h>
#undef _OSS_GRANT_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include <ossc/modules/oss_initiate_multipart_upload_request.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include <ossc/modules/oss_initiate_multipart_upload_result.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H

#define _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#include <ossc/modules/oss_list_multipart_uploads_request.h>
#undef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

#define _OSS_LIST_OBJECTS_REQUEST_H
#include <ossc/modules/oss_list_objects_request.h>
#undef _OSS_LIST_OBJECTS_REQUEST_H

#define _OSS_LIST_PARTS_REQUEST_H
#include <ossc/modules/oss_list_parts_request.h>
#undef _OSS_LIST_OBJECTS_REQUEST_H

#define _OSS_MULTIPART_OBJECT_GROUP_H
#include <ossc/modules/oss_multipart_object_group.h>
#undef _OSS_MULTIPART_OBJECT_GROUP_H

#define _OSS_MULTIPART_UPLOAD_H
#include <ossc/modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H

#define _OSS_MULTIPART_UPLOAD_LISTING_H
#include <ossc/modules/oss_multipart_upload_listing.h>
#undef _OSS_MULTIPART_UPLOAD_LISTING_H

#define _OSS_OBJECT_H
#include <ossc/modules/oss_object.h>
#define _OSS_OBJECT_H

#define _OSS_OBJECT_GROUP_ITEM_H
#include <ossc/modules/oss_object_group_item.h>
#undef _OSS_OBJECT_GROUP_ITEM_H

#define _OSS_OBJECT_LISTING_H
#include <ossc/modules/oss_object_listing.h>
#undef _OSS_OBJECT_LISTING_H

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

#define _OSS_OBJECT_SUMMARY_H
#include <ossc/modules/oss_object_summary.h>
#undef _OSS_OBJECT_SUMMARY_H

#define _OSS_OWNER_H
#include <ossc/modules/oss_owner.h>
#undef _OSS_OWNER_H

#define _OSS_PART_ETAG_H
#include <ossc/modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H

#define _OSS_PART_LISTING_H
#include <ossc/modules/oss_part_listing.h>
#undef _OSS_PART_LISTING_H

#define _OSS_PART_SUMMARY_H
#include <ossc/modules/oss_part_summary.h>
#undef _OSS_PART_SUMMARY_H

#define _OSS_POST_OBJECT_GROUP_REQUEST_H
#include <ossc/modules/oss_post_object_group_request.h>
#undef _OSS_POST_OBJECT_GROUP_REQUEST_H

#define _OSS_POST_OBJECT_GROUP_RESULT_H
#include <ossc/modules/oss_post_object_group_result.h>
#undef _OSS_POST_OBJECT_GROUP_RESULT_H

#define _OSS_PUT_OBJECT_RESULT_H
#include <ossc/modules/oss_put_object_result.h>
#undef _OSS_PUT_OBJECT_RESULT_H

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <ossc/modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#define _OSS_UPLOAD_PART_REQUEST_H
#include <ossc/modules/oss_upload_part_request.h>
#undef _OSS_UPLOAD_PART_REQUEST_H

#define _OSS_UPLOAD_PART_RESULT_H
#include <ossc/modules/oss_upload_part_result.h>
#undef _OSS_UPLOAD_PART_RESULT_H

#include <ossc/util/oss_ttxml.h>
#include <ossc/util/oss_tstring.h>
/**
 * @defgroup oss_client_t oss_client_t
 * @{
 */

/**
 * 访问阿里云开放存储服务（Open Storage Service， OSS）的入口。
 * 阿里云存储服务（Open Storage Service，简称OSS），是阿里云对外提供的海量，
 * 安全，低成本， 高可靠的云存储服务。用户可以通过简单的REST接口，
 * 在任何时间、任何地点上传和下载数据， 也可以使用WEB页面对数据进行管理。
 * 基于OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等
 * 基于大规模数据的服务。 
 */
typedef struct oss_client_s {
	char *endpoint;       /**< hostname */
	char *access_id;      /**< 用户的OSS服务用户名 */
	char *access_key;     /**< 用户的OSS服务密码 */
}oss_client_t;

/**
 * struct curl_request_param_s 内部使用 \n
 * curl_request_param_t 开发者使用
 */
typedef struct curl_request_param_s curl_request_param_t;

/**
 * param_buffer_t中buffer的内容
 */
struct param_buffer_s {
	char *ptr; /**< 缓冲区首指针 */
	FILE *fp; /**< 文件指针 */
	size_t left; /** 缓冲区剩余大小 */
	size_t allocated; /** 缓冲区总大小 */
	unsigned short code; /**返回码 */
};

/**
 * struct param_buffer_s 内部使用 \n
 * param_buffer_t 开发者使用
 */
typedef struct param_buffer_s param_buffer_t;

/**
 * http请求与接受的buffer总体结构
 */
struct curl_request_param_s {
	param_buffer_t *send_buffer; /**< send buffer */
	param_buffer_t *recv_buffer; /**< receive buffer */
	param_buffer_t *header_buffer; /**< header buffer */
};

/**
 * oss_client_t 的构造函数
 * @param access_id [in] 用户的OSS服务用户名
 * @param access_key [in] 用户的OSS服务密码
 * @return 返回一个标识该用户身份的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_client_t *
client_initialize(
		const char *access_id,
		const char *access_key);

/**
 * oss_client_t 带endpoint参数的构造函数
 * @param access_id [in] 用户的OSS服务用户名
 * @param access_key [in] 用户的OSS服务密码
 * @param endpoint [in] 标识hostname
 * @return 返回一个标识该用户身份的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_client_t *
client_initialize_with_endpoint(
		const char *access_id,
		const char *access_key,
		const char *endpoint);

/**
 * oss_client_t 析构函数
 * @param client [in] 标识一个oss_client_t的结构指针
 * @retval void
 * @pre client 必须使用client_initialize的返回值
 */
extern void 
client_finalize(oss_client_t *client);

/**
 * 终止一个 Multipart 上传事件
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_abort_multipart_upload_request的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @retval void
 */
extern void
client_abort_multipart_upload(oss_client_t *client,
		oss_abort_multipart_upload_request_t *request,
		unsigned short *retcode);

/**
 * 完成一个 Multipart 上传事件
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_complete_multipart_upload_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_complete_multipart_upload_result_t *
client_complete_multipart_upload(oss_client_t *client,
		oss_complete_multipart_upload_request_t *request,
		unsigned short *retcode);

/**
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_copy_object_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_copy_object_result_t *
client_copy_object(oss_client_t *client,
		oss_copy_object_request_t *request,
		unsigned short *retcode);

/**
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param source_bucket_name 被拷贝的Object所在的Bucket的名称
 * @param source_key 被拷贝的Object的名称
 * @param destination_bucket_name 目标Object所在的Bucket的名称
 * @param destination_key 目标Object的名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_copy_object_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_copy_object_result_t *
client_copy_object_ext(oss_client_t *client,
		const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key,
		unsigned short *retcode);

/**
 * 创建 Bucket
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要创建的Bucket的名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @retval void
 */
extern void
client_create_bucket(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode);

/**
 * 删除 Bucket
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要删除的Bucket的名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @retval void
 */
extern void 
client_delete_bucket(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode);

/**
 * 删除指定的 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要删除Object所在的Bucket的名称
 * @param key [in] 要删除Object的名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @retval void
 */
extern void
client_delete_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode);

#if 0
/**
 * 生成一个包含签名信息并可以访问 OSSObject 的 URL
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_generate_presigned_url_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return url字符串
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern char *
client_generate_presigned_url(oss_client_t *client,
		oss_generate_presigned_url_request_t *request);
#endif

/**
 * 生成一个用 HTTP GET 方法访问 OSSObject 的 URL
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要访问的Object所在的Bucket名称
 * @param key [in] 要访问的Object的名称
 * @param expiration [in] 过期时间
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return url字符串
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern char *
client_generate_presigned_url_with_expiration(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration);

/**
 * 生成一个用指定 HTTP 方法访问 OSSObject 的 URL
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要访问的Object所在的Bucket名称
 * @param key [in] 要访问的Object的名称
 * @param expiration [in] 过期时间
 * @param method [in] http方法
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return url字符串
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern char *
client_generate_presigned_url_with_method(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration,
		const char *method);

/**
 * 返回给定 Bucket 的 Access Control List(oss_client_t *client, ACL)
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name [in] 要设置acl的Bucket名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_access_control_list_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_access_control_list_t *
client_get_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode);

/**
 * 返回访问的OSS Endpoint
 * @param client [in] 标识一个oss_client_t的结构指针
 * @return 服务器的hostname
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern const char *
client_get_endpoint(oss_client_t *client);

/**
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_get_object_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_t *
client_get_object(oss_client_t *client,
		oss_get_object_request_t *request,
		unsigned short *retcode);

/**
 * 从 OSS 指定的 Bucket 中导出指定的 OSSObject 到目标文件
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_get_object_request_t的结构指针
 * @param file [in] 要存放Object的文件指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_metadata_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_metadata_t *
client_get_object_to_file(oss_client_t *client,
		oss_get_object_request_t *request,
		FILE *file,
		unsigned short *retcode);

/**
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要导出的Object所在的Bucket名称
 * @param key 要导出的Object名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_t *
client_get_object_with_bucket_name(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode);

/**
 * 返回 OSSObject 的元数据
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要查询的Object所在的Bucket名称
 * @param key 要查询的Object名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_metadata_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_metadata_t *
client_get_object_metadata(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode);
/**
 * 初始化一个 Multipart 上传事件
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_initiate_multipart_upload_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_initiate_multipart_upload_result_t *
client_initiate_multipart_upload(oss_client_t *client,
		oss_initiate_multipart_upload_request_t *request,
		unsigned short *retcode);


/**
 * 判断给定 Bucket 是否存在
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要查询的Bucket名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 真或假
 * @retval true 表示存在
 * @retval false 表示不存在
 */
extern bool
client_is_bucket_exist(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode);

/**
 * 返回请求者拥有的所有 Bucket 的列表
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param buckets_number 所有Bucket的总数
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_bucket_t的双重指针，可通过buckets_number来遍历
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_bucket_t **
client_list_buckets(oss_client_t *client,
		int *buckets_number,
		unsigned short *retcode);

/**
 * 列出所有执行中的 Multipart 上传事件
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_list_multipart_uploads_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_multipart_upload_listing_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_multipart_upload_listing_t *
client_list_multipart_uploads(oss_client_t *client,
		oss_list_multipart_uploads_request_t *request,
		unsigned short *retcode);

/**
 * 列出指定 Bucket 下的 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_list_objects_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_listing_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_listing_t *
client_list_objects(oss_client_t *client,
		oss_list_objects_request_t *request,
		unsigned short *retcode);

/**
 * 列出指定 Bucket 下的 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要查询的Bucket的名称
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_listing_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_listing_t *
client_list_objects_with_bucket_name(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode);

/**
 * 列出指定 Bucket 下 key 以给定 prefix 开头的 OSSObject
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要查询的Bucket的名称
 * @param prefix 设置要查询的Object的前缀
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_object_listing_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_object_listing_t *
client_list_objects_with_prefix(oss_client_t *client,
		const char *bucket_name,
		const char *prefix,
		unsigned short *retcode);

/**
 * 列出 multipart 中上传的所有 part 信息
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_list_parts_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_part_listing_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_part_listing_t *
client_list_parts(oss_client_t *client,
		oss_list_parts_request_t *request,
		unsigned short *retcode);

/**
 * 上传指定的 OSSObject 到 OSS 中指定的 Bucket
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要上传到的Bucket名称
 * @param key 要上传到服务器上显示的Object的名称
 * @param input 要上传的文件指针
 * @param metadata 标识数据的一些元信息，一个oss_object_metadata_t结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_put_object_result_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_put_object_result_t *
client_put_object_from_file(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input,
		oss_object_metadata_t *metadata,
		unsigned short *retcode);

/**
 * 上传指定的 OSSObject 到 OSS 中指定的 Bucket
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要上传到的Bucket名称
 * @param key 要上传到服务器上显示的Object的名称
 * @param input 要上传数据的缓存区
 * @param metadata 标识数据的一些元信息，一个oss_object_metadata_t结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_put_object_result_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_put_object_result_t *
client_put_object_from_buffer(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input,
		oss_object_metadata_t *metadata,
		unsigned short *retcode);

/**
 * 设置指定 Bucket 的 Access Control List(oss_client_t *client, ACL)
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param bucket_name 要设置权限的Bucket的名称
 * @param acl 要设置的权限（private, public-read, public-read-write中的一个）
 * @param retcode [out] 服务器返回的HTTP返回码
 * @retval void
 */
extern void
client_set_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		const char *acl,
		unsigned short *retcode);

/**
 * 上传一个分块（Part）到指定的的 Multipart 上传事件中
 * @param client [in] 标识一个oss_client_t的结构指针
 * @param request [in] 标识一个oss_upload_part_request_t的结构指针
 * @param retcode [out] 服务器返回的HTTP返回码
 * @return 返回一个oss_upload_part_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_upload_part_result_t *
client_upload_part(oss_client_t *client,
		oss_upload_part_request_t *request,
		unsigned short *retcode);
/**@}*/
#endif // OSS_CLIENT_H
