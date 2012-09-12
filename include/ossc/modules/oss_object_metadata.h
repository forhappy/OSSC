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

typedef struct oss_object_metadata_s oss_object_metadata_t;
struct oss_object_metadata_s {
	oss_map_t *metadata;
	oss_map_t *user_metadata;

	/**
	 * 添加一个用户自定义的元数据
	 */
	void (*add_user_metadata)(oss_object_metadata_t *metadata,
			const char *key,
			const char *value);

	/**
	 * 获取Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
	 */
	const char * (*get_cache_control)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
	 */
	const char * (*get_content_disposition)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Encoding请求头，表示Object内容的编码方式
	 **/
	const char * (*get_content_encoding)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Length请求头，表示Object内容的大小
	 */
	long (*get_content_length)(oss_object_metadata_t *metadata);

	/**
	 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
	 */
	const char * (*get_content_type)(oss_object_metadata_t *metadata);

	/**
	 * 获取一个值表示与Object相关的hex编码的128位MD5摘要
	 */

	const char * (*get_etag)(oss_object_metadata_t *metadata);

	/**
	 * 获取Expires请求头
	 */
	const char * (*get_expiration_time)(oss_object_metadata_t *metadata);

	/**
	 * 获取Last-Modified请求头的值，表示Object最后一次修改的时间
	 */
	const char * (*get_last_modified)(oss_object_metadata_t *metadata);

	/**
	 * 返回内部保存的请求头的元数据（内部使用）
	 */
	oss_map_t * (*get_raw_metadata)(oss_object_metadata_t *metadata);

	/**
	 * 获取用户自定义的元数据
	 */
	oss_map_t * (*get_user_metadata)(oss_object_metadata_t *metadata);

	/**
	 * 设置Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
	 */
	void (*set_cache_control)(oss_object_metadata_t *metadata, 
			const char *cache_control);

	/**
	 * 设置Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
	 */
	void (*set_content_disposition)(oss_object_metadata_t *metadata,
			const char *disposition);

	/**
	 * 设置Content-Encoding请求头，表示Object内容的编码方式
	 */
	void (*set_content_encoding)(oss_object_metadata_t *metadata,
			const char *encoding);

	/**
	 *  设置Content-Length请求头，表示Object内容的大小
	 */
	void (*set_content_length)(oss_object_metadata_t *metadata,
			long content_length);

	/**
	 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
	 */
	void (*set_content_type)(oss_object_metadata_t *metadata,
			const char *content_type);

	/**
	 *  设置Expires请求头
	 */
	void (*set_expiration_time)(oss_object_metadata_t *metadata,
			const char *expiration_time);

	/**
	 *  设置请求头（内部使用）
	 */
	void (*set_header)(oss_object_metadata_t *metadata,
			const char *key,
			const char *value);

	/**
	 * 设置Last-Modified请求头的值，表示Object最后一次修改的时间（内部使用）
	 */
	void (*set_last_modified)(oss_object_metadata_t *metadata,
			const char *last_modified);

	/**
	 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
	 */
	void (*set_user_metadata)(oss_object_metadata_t *metadata,
			oss_map_t *user_metadata);
};

/**
 * 初始化构造函数
 */
extern oss_object_metadata_t *
object_metadata_initialize();

/**
 * 析构函数
 */
extern void
object_metadata_finalize(oss_object_metadata_t *metadata);

#endif // OSS_OBJECT_METADATA_H

