/*
 * =============================================================================
 *
 *       Filename:  oss_client.c
 *
 *    Description:  oss client
 *
 *        Created:  09/03/2012 09:18:04 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <oss_client.h>


/* *
 * 终止一个 Multipart 上传事件
 * */
void
client_abort_multipart_upload(oss_abort_multipart_request_t *request)
{}

/* *
 * 完成一个 Multipart 上传事件
 * */
oss_complete_multipart_upload_result_t *
client_complete_multipart_upload(oss_complete_multipart_upload_request_t *request)
{}

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
oss_copy_object_result_t *
client_copy_object(oss_copy_object_request_t *request)
{}

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
oss_copy_object_result_t *
client_copy_object_ext(const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key)
{}

/* *
 * 创建 Bucket
 * */
oss_bucket_t *
client_create_bucket(const char *bucket_name)
{}

/* *
 * 删除 Bucket
 * */
void
client_delete_bucket(const char *bucket_name)
{}

/* *
 * 删除指定的 OSSObject
 * */
void
client_delete_object(const char *bucket_name,
		const char *key)
{}

/* *
 * 生成一个包含签名信息并可以访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url(oss_generate_presigned_url_request_t *request)
{}

/* *
 * 生成一个用 HTTP GET 方法访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url_with_expiration(const char *bucket_name,
		const char *key,
		const char *expiration)
{}

/* *
 * 生成一个用指定 HTTP 方法访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url_with_method(const char *bucket_name,
		const char *key,
		const char *expiration,
		oss_http_method_t method)
{}

/* *
 * 返回给定 Bucket 的 Access Control List(ACL)
 * */
oss_access_control_list_t *
client_get_bucket_acl(const char *bucket_name)
{}

/* *
 * 返回访问的OSS Endpoint
 * */
const char *
client_get_endpoint(void)
{}

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
oss_object_t *
client_get_object(oss_get_object_request_t *request)
{}

/* *
 * 从 OSS 指定的 Bucket 中导出指定的 OSSObject 到目标文件
 * */
oss_object_metadata_t *
client_get_object_to_file(oss_get_object_request_t *request,
		FILE *file)
{}

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
oss_object_t *
client_get_object_with_bucket_name(const char *bucket_name,
		const char *key)
{}

/* *
 * 返回 OSSObject 的元数据
 * */
oss_object_metadata_t *
client_get_object_metadata(const char *bucket_name,
		const char *key)
{}


/* *
 * 初始化 oss_client_t
 * */
bool
client_initialize(const char *access_id,
		const char *access_key)
{}

/* *
 * 初始化 oss_client_t
 * */
bool
client_initialize_with_endpoint(const char *access_id,
		const char *access_key,
		const char *endpoint)
{}

/* *
 * 初始化一个 Multipart 上传事件
 * */
oss_initiate_multipart_upload_result_t *
client_initiate_multipart_upload(oss_initiate_multipart_upload_request_t *request)
{}

/* *
 * 判断给定 Bucket 是否存在
 * */
bool
client_is_bucket_exist(const char *bucket_name)
{}

/* *
 * 返回请求者拥有的所有 Bucket 的列表
 * */
oss_bucket_t *
client_list_buckets(void)
{}

/* *
 * 列出所有执行中的 Multipart 上传事件
 * */
oss_multipart_upload_listing_t *
client_list_multipart_uploads(oss_list_multipart_uploads_request_t *request)
{}

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
oss_object_listing_t *
client_list_objects(oss_list_objects_request *request)
{}

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
oss_object_listing_t *
client_list_objects_with_bucket_name(const char *bucket_name)
{}

/* *
 * 列出指定 Bucket 下 key 以给定 prefix 开头的 OSSObject
 * */
oss_object_listing_t *
client_list_objects_with_prefix(const char *bucket_name,
		const char *prefix)
{}

/* *
 * 列出 multipart 中上传的所有 part 信息
 * */
oss_part_listing_t *
client_list_parts(oss_list_parts_request_t *request)
{}

/* *
 * 上传指定的 OSSObject 到 OSS 中指定的 Bucket
 * */
oss_put_object_result_t *
client_put_object(const char *bucket_name,
		const char *key,
		const char *input,
		oss_object_metadata_t *metadata)
{}

/* *
 * 设置指定 Bucket 的 Access Control List(ACL)
 * */
void
client_set_bucket_acl(const char *bucket_name,
		oss_canned_access_control_list_t *acl)
{}

/* *
 * 上传一个分块（Part）到指定的的 Multipart 上传事件中
 * */
oss_upload_part_result_t *
client_upload_part(oss_upload_part_request_t *request)
{}
