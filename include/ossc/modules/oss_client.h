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
# error Never include <modules/oss_client.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_CLIENT_H
#define OSS_CLIENT_H

#define _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
#include <modules/oss_abort_multipart_upload_request.h>
#undef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_ACCESS_CONTROL_LIST_H
#include <modules/oss_access_control_list.h>
#undef _oss_access_control_list_h

#define _OSS_BUCKET_H
#include <modules/oss_bucket.h>
#undef _OSS_BUCKET_H

#define _OSS_CLIENT_H
#include <modules/oss_client.h>
#undef _OSS_CLIENT_H

#define _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#include <modules/oss_complete_multipart_upload_request.h>
#undef _oss_complete_multipart_upload_request_h

#define _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include <modules/oss_complete_multipart_upload_result.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H

#define _OSS_COPY_OBJECT_REQUEST_H
#include <modules/oss_copy_object_request.h>
#undef _OSS_COPY_OBJECT_REQUEST_H

#define _OSS_COPY_OBJECT_RESULT_H
#include <modules/oss_copy_object_result.h>
#undef _OSS_COPY_OBJECT_RESULT_H

#define _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include <modules/oss_generate_presigned_url_request.h>
#undef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H

#define _OSS_GET_OBJECT_REQUEST_H
#include <modules/oss_get_object_request.h>
#undef _OSS_GET_OBJECT_REQUEST_H

#define _OSS_GRANT_H
#include <modules/oss_grant.h>
#undef _OSS_GRANT_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include <modules/oss_initiate_multipart_upload_request.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include <modules/oss_initiate_multipart_upload_result.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H

#define _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#include <modules/oss_list_multipart_uploads_request.h>
#undef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

#define _OSS_LIST_OBJECTS_REQUEST_H
#include <modules/oss_list_objects_request.h>
#undef _OSS_LIST_OBJECTS_REQUEST_H

#define _OSS_LIST_PARTS_REQUEST_H
#include <modules/oss_list_parts_request.h>
#undef _OSS_LIST_OBJECTS_REQUEST_H

#define _OSS_MULTIPART_UPLOAD_H
#include <modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H

#define _OSS_MULTIPART_UPLOAD_LISTING_H
#include <modules/oss_multipart_upload_listing.h>
#undef _OSS_MULTIPART_UPLOAD_LISTING_H

#define _OSS_OBJECT_H
#include <modules/oss_object.h>
#define _OSS_OBJECT_H

#define _OSS_OBJECT_LISTING_H
#include <modules/oss_object_listing.h>
#undef _OSS_OBJECT_LISTING_H

#define _OSS_OBJECT_METADATA_H
#include <modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

#define _OSS_OBJECT_SUMMARY_H
#include <modules/oss_object_summary.h>
#undef _OSS_OBJECT_SUMMARY_H

#define _OSS_OWNER_H
#include <modules/oss_owner.h>
#undef _OSS_OWNER_H

#define _OSS_PART_ETAG_H
#include <modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H

#define _OSS_PART_LISTING_H
#include <modules/oss_part_listing.h>
#undef _OSS_PART_LISTING_H

#define _OSS_PART_SUMMARY_H
#include <modules/oss_part_summary.h>
#undef _OSS_PART_SUMMARY_H

#define _OSS_PUT_OBJECT_RESULT_H
#include <modules/oss_put_object_result.h>
#undef _OSS_PUT_OBJECT_RESULT_H

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#define _OSS_UPLOAD_PART_REQUEST_H
#include <modules/oss_upload_part_request.h>
#undef _OSS_UPLOAD_PART_REQUEST_H

#define _OSS_UPLOAD_PART_RESULT_H
#include <modules/oss_upload_part_result.h>
#undef _OSS_UPLOAD_PART_RESULT_H

/* *
 * 访问阿里云开放存储服务（Open Storage Service， OSS）的入口。
 * 阿里云存储服务（Open Storage Service，简称OSS），是阿里云对外提供的海量，
 * 安全，低成本， 高可靠的云存储服务。用户可以通过简单的REST接口，
 * 在任何时间、任何地点上传和下载数据， 也可以使用WEB页面对数据进行管理。
 * 基于OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等
 * 基于大规模数据的服务。 
 * */

typedef struct oss_client_s {
	char *endpoint; // Host;
	char *access_id;
	char *access_key;
}oss_client_t;

/* *
 * 初始化 oss_client_t
 * */
extern oss_client_t *
client_initialize(
		const char *access_id,
		const char *access_key);

/* *
 * 初始化 oss_client_t
 * */
extern oss_client_t *
client_initialize_with_endpoint(
		const char *access_id,
		const char *access_key,
		const char *endpoint);
/* *
 * 终止一个 Multipart 上传事件
 * */
extern void
client_abort_multipart_upload(oss_client_t *client,
		oss_abort_multipart_upload_request_t *request);

/* *
 * 完成一个 Multipart 上传事件
 * */
extern oss_complete_multipart_upload_result_t *
client_complete_multipart_upload(oss_client_t *client,
		oss_complete_multipart_upload_request_t *request);

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
extern oss_copy_object_result_t *
client_copy_object(oss_client_t *client,
		oss_copy_object_request_t *request);

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
extern oss_copy_object_result_t *
client_copy_object_ext(oss_client_t *client,
		const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key);

/* *
 * 创建 Bucket
 * */
extern oss_bucket_t *
client_create_bucket(oss_client_t *client,
		const char *bucket_name);

/* *
 * 删除 Bucket
 * */
extern void
client_delete_bucket(oss_client_t *client,
		const char *bucket_name);

/* *
 * 删除指定的 OSSObject
 * */
extern void
client_delete_object(oss_client_t *client,
		const char *bucket_name,
		const char *key);

/* *
 * 生成一个包含签名信息并可以访问 OSSObject 的 URL
 * */
extern const char *
client_generate_presigned_url(oss_client_t *client,
		oss_generate_presigned_url_request_t *request);

/* *
 * 生成一个用 HTTP GET 方法访问 OSSObject 的 URL
 * */
extern const char *
client_generate_presigned_url_with_expiration(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration);

/* *
 * 生成一个用指定 HTTP 方法访问 OSSObject 的 URL
 * */
extern const char *
client_generate_presigned_url_with_method(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration,
		const char *method);

/* *
 * 返回给定 Bucket 的 Access Control List(oss_client_t *client, ACL)
 * */
extern oss_access_control_list_t *
client_get_bucket_acl(oss_client_t *client,
		const char *bucket_name);

/* *
 * 返回访问的OSS Endpoint
 * */
extern const char *
client_get_endpoint(oss_client_t *client);

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
extern oss_object_t *
client_get_object(oss_client_t *client,
		oss_get_object_request_t *request);

/* *
 * 从 OSS 指定的 Bucket 中导出指定的 OSSObject 到目标文件
 * */
extern oss_object_metadata_t *
client_get_object_to_file(oss_client_t *client,
		oss_get_object_request_t *request,
		FILE *file);

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
extern oss_object_t *
client_get_object_with_bucket_name(oss_client_t *client,
		const char *bucket_name,
		const char *key);

/* *
 * 返回 OSSObject 的元数据
 * */
extern oss_object_metadata_t *
client_get_object_metadata(oss_client_t *client,
		const char *bucket_name,
		const char *key);
/* *
 * 初始化一个 Multipart 上传事件
 * */
extern oss_initiate_multipart_upload_result_t *
client_initiate_multipart_upload(oss_client_t *client,
		oss_initiate_multipart_upload_request_t *request);


/* *
 * 判断给定 Bucket 是否存在
 * */
extern bool
client_is_bucket_exist(oss_client_t *client,
		const char *bucket_name);

/* *
 * 返回请求者拥有的所有 Bucket 的列表
 * */
extern oss_bucket_t *
client_list_buckets(oss_client_t *client);

/* *
 * 列出所有执行中的 Multipart 上传事件
 * */
extern oss_multipart_upload_listing_t *
client_list_multipart_uploads(oss_client_t *client,
		oss_list_multipart_uploads_request_t *request);

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
extern oss_object_listing_t *
client_list_objects(oss_client_t *client,
		oss_list_objects_request_t *request);

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
extern oss_object_listing_t *
client_list_objects_with_bucket_name(oss_client_t *client,
		const char *bucket_name);

/* *
 * 列出指定 Bucket 下 key 以给定 prefix 开头的 OSSObject
 * */
extern oss_object_listing_t *
client_list_objects_with_prefix(oss_client_t *client,
		const char *bucket_name,
		const char *prefix);

/* *
 * 列出 multipart 中上传的所有 part 信息
 * */
extern oss_part_listing_t *
client_list_parts(oss_client_t *client,
		oss_list_parts_request_t *request);

/* *
 * 上传指定的 OSSObject 到 OSS 中指定的 Bucket
 * */
extern oss_put_object_result_t *
client_put_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input,
		oss_object_metadata_t *metadata);

/* *
 * 设置指定 Bucket 的 Access Control List(oss_client_t *client, ACL)
 * */
extern void
client_set_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		const char *acl);

/* *
 * 上传一个分块（Part）到指定的的 Multipart 上传事件中
 * */
extern oss_upload_part_result_t *
client_upload_part(oss_client_t *client,
		oss_upload_part_request_t *request);

#endif // OSS_CLIENT_H
