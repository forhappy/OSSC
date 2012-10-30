/*
 * =============================================================================
 *
 *       Filename:  oss_constants.h
 *
 *    Description: oss constants definition. 
 *
 *        Created:  09/04/2012 03:21:26 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_CONSTANTS_H
# error Never include <ossc/oss_constants.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_CONSTANTS_H
#define OSS_CONSTANTS_H

typedef enum _OSS_ERROR_CODE {
	/* 0 返回成功 */
	OK,
	/* 1 删除成功 */
	NO_CONTENT,
	/* 拒绝访问 */
	ACCESS_DENIED,
	/* Bucket 已经存在 */
	BUCKET_ALREADY_EXISTS,
	/* Bucket 不为空 */
	BUCKET_NOT_EMPTY,
	/* 实体过大 */
	ENTITY_TOO_LARGE,
	/* 实体过小 */
	ENTITY_TOO_SMALL,
	/* 文件组过大 */
	FILE_GROUP_TOO_LARGE,
	/* 文件part不存在 */
	FILE_PART_NOT_EXIST,
	/* 文件Part过时 */
	FILE_PART_STALE,
	/* OSS 内部发生错误 */
	INTERNAL_ERROR,
	/* Access ID不存在 */
	INVALID_ACCESS_KEY_ID,
	/* 参数格式错误 */
	INVALID_ARGUMENT,
	/* 无效的 Bucket 名字 */
	INVALID_BUCKET_NAME,
	/* 无效的摘要 */
	INVALID_DIGEST,
	/* 无效的 Object 名字 */
	INVALID_OBJECT_NAME,
	/* 无效的 Part */
	INVALID_PART,
	/* 无效的 Part顺序 */
	INVALID_PART_ORDER,
	/* XML格式非法 */
	MALFORMED_XML,
	/* 不支持的方法 */
	METHOD_NOT_ALLOWED,
	/* 缺少参数 */
	MISSING_ARGUMENT,
	/* 缺少内容长度 */
	MISSING_CONTENT_LENGTH,
	/* Bucket 不存在 */
	NO_SUCH_BUCKET,
	/* 文件不存在 */
	NO_SUCH_KEY,
	/* Multipart Upload ID 不存在 */
	NO_SUCH_UPLOAD,
	/* 无法处理的方法 */
	NOT_IMPLEMENTED,
	/* 预处理错误 */
	PRECONDITION_FAILED,
	/* 发起请求的时间和服务器时间超出15分钟 */
	REQUEST_TIME_TOO_SKEWED,
	/* 请求超时 */
	REQUEST_TIMEOUT,
	/* 签名错误 */
	SIGNATURE_DOES_NOT_MATCH,
	/* 用户的 Bucket 数目超过限制 */
	TOO_MANY_BUCKETS,
	/* 文件完整性 */
	FILE_PART_INTERITY,
	/* 文件找不到 */
	FILE_NOT_FOUND,
	/* 没有被修改 */
	NOT_MODIFIED,
	/* 文件过大 */
	FILE_TOO_LARGE,
	/* IO 错误 */
	IO_ERROR

}OSS_ERROR_CODE;

typedef enum _OSS_PERMISSION {
	FULLCONTROL,
	READ
}OSS_PERMISSION;

#define DEFAULT_OSS_HOST            "oss.aliyuncs.com"
#define VERSION                     "0.1.6"
#define NAME                        "OSS-C-SDK"

#define OSS_BUCKET                  "bucket"
#define OSS_OBJECT                  "object"
#define OSS_HEADERS                 "headers"
#define OSS_METHOD                  "method"
#define OSS_QUERY                   "query"
#define OSS_BASENAME                "basename"
#define OSS_MAX_KEYS                "max-keys"
#define OSS_UPLOAD_ID               "uploadId"
#define OSS_MAX_KEYS_VALUE          100
#define OSS_MAX_OBJECT_GROUP_VALUE  1000
#define OSS_FILE_SLICE_SIZE         8192
#define OSS_PREFIX                  "prefix"
#define OSS_DELIMITER               "delimiter"
#define OSS_MARKER                  "marker"
#define OSS_CONTENT_MD5             "Content-Md5"
#define OSS_CONTENT_TYPE            "Content-Type"
#define OSS_CONTENT_LENGTH          "Content-Length"
#define OSS_IF_MODIFIED_SINCE       "If-Modified-Since"
#define OSS_IF_UNMODIFIED_SINCE     "If-Unmodified-Since"
#define OSS_IF_MATCH                "If-Match"
#define OSS_IF_NONE_MATCH           "If-None-Match"
#define OSS_CACHE_CONTROL           "Cache-Control"
#define OSS_EXPIRES                 "Expires"
#define OSS_PREAUTH                 "preauth"
#define OSS_CONTENT_ENCOING         "Content-Encoding"
#define OSS_CONTENT_DISPOSITION     "Content-Disposition"
#define OSS_RANGE                   "Range"
#define OS_CONTENT_RANGE            "Content-Range"
#define OSS_CONTENT                 "content"
#define OSS_BODY                    "body"
#define OSS_LENGTH                  "length"
#define OSS_HOST                    "Host"
#define OSS_DATE                    "Date"
#define OSS_AUTHORIZATION           "Authorization"
#define OSS_LAST_MODIFIED           "Last-Modified"

#define OSS_FILE_DOWNLOAD           "fileDownload"
#define OSS_FILE_UPLOAD             "fileUpload"
#define OSS_PART_SIZE               "partSize"
#define OSS_SEEK_TO                 "seekTo"
#define OSS_SIZE                    "size"
#define OSS_QUERY_STRING            "query_string"
#define OSS_SUB_RESOURCE            "sub_resource"
#define OSS_DEFAULT_PREFIX          "x-oss-"

#define OSS_URL_ACCESS_KEY_ID  "OSSAccessKeyId"
#define OSS_URL_EXPIRES        "Expires"
#define OSS_URL_SIGNATURE      "Signature"

//HTTP方法
#define OSS_HTTP_GET     "GET"
#define OSS_HTTP_PUT     "PUT"
#define OSS_HTTP_HEAD    "HEAD"
#define OSS_HTTP_POST    "POST"
#define OSS_HTTP_DELETE  "DELETE"

//其他常量
#define OSS_ACL                         "x-oss-acl"
#define OSS_OBJECT_GROUP                "x-oss-file-group"
#define OSS_COPY_SOURCE					"x-oss-copy-source"
#define OSS_MULTI_PART                  "uploads"
#define OSS_MULTI_DELETE                "delete"
#define OSS_OBJECT_COPY_SOURCE          "x-oss-copy-source"
#define OSS_ACL_TYPE_PRIVATE            "private"
#define OSS_ACL_TYPE_PUBLIC_READ        "public-read"
#define OSS_ACL_TYPE_PUBLIC_READ_WRITE  "public-read-write"

//ResponseHeaderOverrides
#define RESPONSE_HEADER_CACHE_CONTROL        "RESPONSE_HEADER_CACHE_CONTROL"
#define RESPONSE_HEADER_CONTENT_DISPOSITION  "RESPONSE_HEADER_CONTENT_DISPOSITION"
#define RESPONSE_HEADER_CONTENT_ENCODING     "RESPONSE_HEADER_CONTENT_ENCODING"
#define RESPONSE_HEADER_CONTENT_LANGUAGE     "RESPONSE_HEADER_CONTENT_LANGUAGE"
#define RESPONSE_HEADER_CONTENT_TYPE         "RESPONSE_HEADER_CONTENT_TYPE"
#define RESPONSE_HEADER_EXPIRES              "RESPONSE_HEADER_EXPIRES"
//
//发送缓冲区，接收缓冲区，头部缓冲区大小
#define MAX_SEND_BUFFER_SIZE			(2 * 1024 * 1024)
#define MAX_RECV_BUFFER_SIZE			(2 * 1024 * 1024)
#define MAX_HEADER_BUFFER_SIZE			(128 * 1024)

#endif // OSS_CONSTANTS_H
