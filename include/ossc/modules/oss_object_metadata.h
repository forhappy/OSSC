/*
 * =============================================================================
 *
 *       Filename:  oss_object_metadata.h
 *
 *    Description:  object metadata structure and implementation.
 *
 *        Created:  09/05/2012 02:45:05 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_OBJECT_METADATA_H
# error Never include <ossc/modules/oss_object_metadata.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_METADATA_H
#define OSS_OBJECT_METADATA_H
#include "ossc-config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ossc/util/oss_map.h>

#define _OSS_CONSTANTS_H
#include <ossc/oss_constants.h>
#undef _OSS_CONSTANTS_H

/**
 * @defgroup oss_object_metadata_t oss_object_metadata_t
 * @{
 */

/**
 * struct oss_object_metadata_s 内部使用 \n
 * oss_object_metadata_t 开发者使用
 */
typedef struct oss_object_metadata_s oss_object_metadata_t;

/**
 * OSS中Object的元数据.包含了用户自定义的元数据,也包含了OSS发送的标准HTTP头(如Content-Length, ETag等）
 */
struct oss_object_metadata_s {
	oss_map_t *metadata;             /**< Http 标准头部 */
	oss_map_t *user_metadata;        /**< Http 用户自定义头部 */

	/**
	 * 增加一个用户自定义头部键值对
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param key [in] 用户自定义头部键
	 * @param value [in] 用户自定义头部值
	 * @retval void
	 */
	void (*add_user_metadata)(oss_object_metadata_t *metadata,
			const char *key,
			const char *value);

	/**
	 * 增加一个标准头部键值对
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param key [in] 标准头部的键
	 * @param value [in] 标准头部的值
	 * @retval void
	 */
	void (*add_default_metadata)(oss_object_metadata_t *metadata,
			const char *key,
			const char *value);

	/**
	 * 获取Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Cache-Control请求头
	 * @retval const char *
	 */
	const char * (*get_cache_control)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Content-Disposition请求头
	 * @retval const char *
	 */
	const char * (*get_content_disposition)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Encoding请求头，表示Object内容的编码方式
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Content-Encoding请求头
	 * @retval const char *
	 */
	const char * (*get_content_encoding)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Length请求头，表示Object内容的大小
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Content-Length请求头
	 * @retval long
	 */
	long (*get_content_length)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Content-Type请求头
	 * @retval const char *
	 */
	const char * (*get_content_type)(oss_object_metadata_t *metadata);

	/**
	 * 获取一个值表示与Object相关的hex编码的128位MD5摘要
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Object的etag值
	 * @retval const char *
	 */

	const char * (*get_etag)(oss_object_metadata_t *metadata);

	/**
	 * 获取Expires请求头
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Expires请求头
	 * @retval const char *
	 */
	const char * (*get_expiration_time)(oss_object_metadata_t *metadata);

	/**
	 * 获取Last-Modified请求头的值，表示Object最后一次修改的时间
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return Last-Modified请求头
	 * @retval const char *
	 */
	const char * (*get_last_modified)(oss_object_metadata_t *metadata);

	/**
	 * 返回内部保存的请求头的元数据（内部使用）
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return 返回一个oss_map_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_map_t * (*get_raw_metadata)(oss_object_metadata_t *metadata);

	/**
	 * 获取用户自定义的元数据
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @return 返回一个oss_map_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_map_t * (*get_user_metadata)(oss_object_metadata_t *metadata);

	/**
	 * 设置Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param cache_control [in] Cache-Control请求头
	 * @retval void
	 */
	void (*set_cache_control)(oss_object_metadata_t *metadata, 
			const char *cache_control);

	/**
	 * 设置Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param disposition [in] Content-Disposition请求头
	 * @retval void
	 */
	void (*set_content_disposition)(oss_object_metadata_t *metadata,
			const char *disposition);

	/**
	 * 设置Content-Encoding请求头，表示Object内容的编码方式
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param encoding [in]Content-Encoding请求头
	 * @retval void
	 */
	void (*set_content_encoding)(oss_object_metadata_t *metadata,
			const char *encoding);

	/**
	 * 设置Content-Length请求头，表示Object内容的大小
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param content_length [in] Content-Length请求头
	 * @retval void
	 */
	void (*set_content_length)(oss_object_metadata_t *metadata,
			long content_length);

	/**
	 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param content_type [in] Content-Type请求头
	 * @retval void
	 */
	void (*set_content_type)(oss_object_metadata_t *metadata,
			const char *content_type);

	/**
	 * 设置Expires请求头
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param expiration_time [in]Expires请求头
	 * @retval void
	 */
	void (*set_expiration_time)(oss_object_metadata_t *metadata,
			const char *expiration_time);

	/**
	 * 设置Object的etag
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param etag [in] Object的etag
	 * @retval void
	 */
	void (*set_etag)(oss_object_metadata_t *metadata, const char *etag);

	/**
	 * 设置请求头（内部使用）
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param key [in] 请求头的键
	 * @param value [in] 请求头的值
	 * @retval void
	 */
	void (*set_header)(oss_object_metadata_t *metadata,
			const char *key,
			const char *value);

	/**
	 * 设置Last-Modified请求头的值，表示Object最后一次修改的时间（内部使用）
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param last_modified [in] Last-Modified请求头
	 * @retval void
	 */
	void (*set_last_modified)(oss_object_metadata_t *metadata,
			const char *last_modified);

	/**
	 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
	 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
	 * @param user_metadata 标识一个oss_map_t结构指针
	 * @retval void
	 */
	void (*set_user_metadata)(oss_object_metadata_t *metadata,
			oss_map_t *user_metadata);
};

/**
 * oss_object_metadata_t构造函数
 * @return 返回一个oss_object_metadata_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_object_metadata_t *
object_metadata_initialize();

/**
 * oss_object_metadata_t析构函数
 * @param metadata [in] 标识一个oss_object_metadata_t结构指针
 * @retval void
 * @pre metadata 必须使用object_metadata_initialize的返回值
 */
extern void
object_metadata_finalize(oss_object_metadata_t *metadata);
/**@}*/
#endif // OSS_OBJECT_METADATA_H

