/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_request.h
 *
 *    Description:  oss copy object request.
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_COPY_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_copy_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COPY_OBJECT_REQUEST_H
#define OSS_COPY_OBJECT_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

/**
 * @defgroup oss_copy_object_request_t oss_copy_object_request_t
 * @{
 */
/**
 * struct oss_copy_object_request_s 内部使用 \n
 * oss_copy_object_request_t 开发者使用
 */
typedef struct oss_copy_object_request_s oss_copy_object_request_t;

/**
 * 拷贝一个在OSS上已经存在的Object成另外一个Object的请求参数
 */
struct oss_copy_object_request_s {
	char *destination_bucket_name;    /**< 目标Bucket的名称 */
	char *destination_key;            /**< 目标Object的名称 */
	char **matching_etag_constraints; /**< ETag限定值的列表 */
	char *modified_since_constraints; /**< 一个时间，如果源Object自从该时间以后被修改过，则执行拷贝操作； 否则抛出异常 */
	oss_object_metadata_t *object_metadata; /**< 目标Object的ObjectMetadata信息 */
	char **no_matching_etag_constraints; /**< 限定Object的ETag限定必须不匹配给定值的列表 */
	char *source_bucket_name; /**< 源Bucket的名称 */
	char *source_key;         /**< 源Object的名称 */
	char *unmodified_since_constraints;   /**< 一个时间，如果该时间等于或者晚于文件实际修改时间，则正常传输文件； 否则抛出异常 */

	unsigned int _counts_matching_etag_constraints;  	 /**< ETag限定值的列表的个数 */
	unsigned int _counts_no_matching_etag_constraints;   /**< 限定Object的ETag限定必须不匹配给定值的列表的个数 */

	/**
	 * 获得目标Bucket的名称
	 * @param request 标识一个oss_copy_object_request_t的结构指针
	 * @return 目标Bucket的名称
	 * @retval const char *
	 */
	const char * (*get_destination_bucket_name)(oss_copy_object_request_t *request);

	/**
	 * 获得目标Object的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return 目标Object的名称
	 * @retval const char *
	 */
	const char * (*get_destination_key)(oss_copy_object_request_t *request);

	/**
	 * 获得ETag限定值的列表
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return ETag限定值的列表
	 * @retval const char **
	 */
	const char ** (*get_matching_etag_constraints)(oss_copy_object_request_t *request,
			unsigned int *counts);

	/**
	 * 获得modified_since_constraints时间
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return modified_since_constraints时间
	 * @retval const char *
	 */
	const char * (*get_modified_since_constraints)(oss_copy_object_request_t *request);

	/**
	 * 获得目标Object的ObjectMetadata信息
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return 目标Object的ObjectMetadata信息
	 * @retval 返回一个oss_object_metadata_t的结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_object_metadata_t * (*get_object_metadata)(oss_copy_object_request_t *request);

	/**
	 * 获得限定Object的ETag限定必须不匹配给定值的列表
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return 限定Object的ETag限定必须不匹配给定值的列表
	 * @retval const char **
	 */
	const char ** (*get_no_matching_etag_constraints)(oss_copy_object_request_t *request,
			unsigned int *counts);

	/**
	 * 获得源Bucket的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return 源Bucket的名称
	 * @retval const char *
	 */
	const char * (*get_source_bucket_name)(oss_copy_object_request_t *request);

	/**
	 * 获得源Object的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return 源Object的名称
	 * @retval const char *
	 */
	const char * (*get_source_key)(oss_copy_object_request_t *request);

	/**
	 * 获得unmodified_since_constraints时间
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @return unmodified_since_constraints时间
	 * @retval const char *
	 */
	const char * (*get_unmodified_since_constraints)(oss_copy_object_request_t *request);


	/**
	 * 设置目标Bucket的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param destination_bucket_name [in] 目标Bucket的名称
	 * @retval void
	 */
	void (*set_destination_bucket_name)(oss_copy_object_request_t *request,
			const char * destination_bucket_name);

	/**
	 * 设置目标Object的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param destination_key [in] 目标Object的名称
	 * @retval void
	 */
	void (*set_destination_key)(oss_copy_object_request_t *request,
			const char *destination_key);

	/**
	 * 设置ETag限定值的列表
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param matching_etag_constraints [in] ETag限定值的列表
	 * @param counts [in] ETag限定值的列表的个数
	 * @retval void
	 */
	void (*set_matching_etag_constraints)(oss_copy_object_request_t *request,
			const char **matching_etag_constraints, unsigned int counts);

	/**
	 * 设置modified_since_constraints时间
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param modified_since_constraints [in] modified_since_constraints时间
	 * @retval void
	 */
	void (*set_modified_since_constraints)(oss_copy_object_request_t *request,
			const char *modified_since_constraints);

	/**
	 * 设置目标Object的ObjectMetadata信息
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param object_metadata [in] 目标Object的ObjectMetadata信息
	 * @retval void
	 */
	void (*set_new_object_metadata)(oss_copy_object_request_t *request,
			oss_object_metadata_t *object_metadata);

	/**
	 * 设置限定Object的ETag限定必须不匹配给定值的列表
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param no_matching_etag_constraints [in] 限定Object的ETag限定必须不匹配给定值的列表
	 * @param counts 限定Object的ETag限定必须不匹配给定值的列表的个数
	 * @retval void
	 */
	void (*set_no_matching_etag_constraints)(oss_copy_object_request_t *request,
			const char **no_matching_etag_constraints, unsigned counts);

	/**
	 * 设置源Bucket的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param source_bucket_name [in] 源Bucket的名称
	 * @retval void
	 */
	void (*set_source_bucket_name)(oss_copy_object_request_t *request,
			const char *source_bucket_name);

	/**
	 * 设置源Object的名称
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param source_key [in] 源Object的名称
	 * @retval void
	 */
	void (*set_source_key)(oss_copy_object_request_t *request,
			const char *source_key);

	/**
	 * 设置unmodified_since_constraints时间
	 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
	 * @param unmodified_since_constraints [in] unmodified_since_constraints时间
	 * @retval void
	 */
	void (*set_unmodified_since_constraints)(oss_copy_object_request_t *request,
			const char *unmodified_since_constraints);
};

/**
 * oss_copy_object_request_t 构造函数
 * @param source_bucket_name [in] 源Bucket的名称
 * @param source_key [in] 源Object的名称
 * @param destination_bucket_name [in] 目标Bucket的名称
 * @param destination_key [in] 目标Object的名称
 * @return 返回一个oss_copy_object_request_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_copy_object_request_t *
copy_object_request_initialize(const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key);

/**
 * oss_copy_object_request_t 析构函数
 * @param request [in] 标识一个oss_copy_object_request_t的结构指针
 * @retval void
 * @pre request 必须使用copy_object_request_initialize的返回值
 */
extern void copy_object_request_finalize(oss_copy_object_request_t *request);
/**@}*/
#endif // OSS_COPY_OBJECT_REQUEST_H 

