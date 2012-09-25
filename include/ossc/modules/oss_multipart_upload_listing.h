/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload_listing.h
 *
 *    Description:  oss_multipart_upload_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_MULTIPART_UPLOAD_LISTING_H
# error Never include <ossc/modules/oss_multipart_upload_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_UPLOAD_LISTING_H
#define OSS_MULTIPART_UPLOAD_LISTING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_MULTIPART_UPLOAD_H
#include <ossc/modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H
#include <ossc/ossc-config.h>

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
/**
 * @defgroup oss_multipart_upload_listing_t oss_multipart_upload_listing_t
 * @{
 */

/**
 * struct oss_multipart_upload_listing_s 内部使用 \n
 * oss_multipart_upload_listing_t 开发者使用
 */
typedef struct oss_multipart_upload_listing_s oss_multipart_upload_listing_t;

/**
 * 包含Multipart上传事件的列表
 */
struct oss_multipart_upload_listing_s {
	char *bucket_name;               /**< Bucket名称 */
	char **common_prefixs;           /**< 包含指定的前缀且第一次出现 delimiter 字符之间的object */
	unsigned int _counts_common_prefixs; /**< common_prefixs的个数 */
	
	char *key_marker;             /**< 标识表示从哪里返回列表 */
	char *upload_id_marker;       /**< 标识表示从哪里返回列表 */
	char *next_key_marker;        /**< 标识返回列表到哪里终止 */
	char *next_upload_id_marker;  /**< 标识返回列表到哪里终止 */
	char *max_uploads;            /**< 返回 Multipart Uploads 事件的最大数目 */

	oss_multipart_upload_t **multipart_uploads;    /**< 各个multipart upload 的信息 */
	unsigned int _counts_multipart_uploads;        /**< multipart upload 的个数*/
	
	bool is_truncated;            /**< 标识是否还有其他multipart upload */
	char *delimiter;              /**< 分隔符*/
	char *prefix;                 /**< 前缀 */

	/**
	 * 获得Bucket名称
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置Bucket名称
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_multipart_upload_listing_t *listing, const char *bucket_name);

	/**
	 * 获得key_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return key_marker
	 * @retval const char *
	 */
	const char * (*get_key_marker)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置key_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param key_marker [in] 标识表示从哪里返回列表
	 * @retval void
	 */
	void (*set_key_marker)(oss_multipart_upload_listing_t *listing, const char *key_marker);

	/**
	 * 获得upload_id_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return upload_id_marker
	 * @retval const char *
	 */
	const char * (*get_upload_id_marker)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置upload_id_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param upload_id_marker [in] 标识表示从哪里返回列表
	 * @retval void
	 */
	void (*set_upload_id_marker)(oss_multipart_upload_listing_t *listing, const char *upload_id_marker);

	/**
	 * 获得next_key_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return next_key_marker
	 * @retval const char *
	 */
	const char * (*get_next_key_marker)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置next_key_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param next_key_marker [in] 标识返回列表到哪里终止
	 * @retval void
	 */
	void (*set_next_key_marker)(oss_multipart_upload_listing_t *listing, const char *next_key_marker);

	/**
	 * 获得next_upload_id_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return next_upload_id_marker
	 * @retval const char *
	 */
	const char * (*get_next_upload_id_marker)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置next_upload_id_marker
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param next_upload_id_marker [in] 标识返回列表到哪里终止
	 * @retval void
	 */
	void (*set_next_upload_id_marker)(oss_multipart_upload_listing_t *listing, const char *next_upload_id_marker);

	/**
	 * 获得max_uploads
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return Multipart Uploads 事件的最大数目
	 * @retval const char *
	 */
	const char * (*get_max_uploads)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置max_uploads
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param max_uploads [in] Multipart Uploads 事件的最大数目
	 * @retval void
	 */
	void (*set_max_uploads)(oss_multipart_upload_listing_t *listing, const char *max_uploads);

	/**
	 * 获得is_truncated
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return is_truncated
	 * @retval bool
	 */
	bool (*get_is_truncated)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置is_truncated
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param is_truncated [in] 标识是否还有其他multipart upload
	 * @retval void
	 */
	void (*set_is_truncated)(oss_multipart_upload_listing_t *listing, bool is_truncated);

	/**
	 * 获得各个multipart upload 的信息
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param counts [out] 所有multipart upload 的信息的数目
	 * @return 返回一个oss_multipart_upload_t结构指针
	 * @retval 非空 标识成功
	 * @retval NULL 标识失败
	 */
	oss_multipart_upload_t ** (*get_multipart_uploads)(oss_multipart_upload_listing_t *listing,
			unsigned int *counts);

	/**
	 * 设置各个multipart upload 的信息
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param multipart_uploads [in] 各个multipart upload 的信息
	 * @param counts [in] 所有multipart upload 的信息的数目
	 * @retval void
	 */
	void (*set_multipart_uploads)(oss_multipart_upload_listing_t *listing,
			oss_multipart_upload_t **multipart_uploads,
			unsigned int counts);

	/**
	 * 获得common_prefixs
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param counts [out] common_prefixs的个数
	 * @return const char **
	 */
	const char ** (*get_common_prefixs)(oss_multipart_upload_listing_t *listing,
			unsigned int *counts);

	/**
	 * 设置common_prefixs
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param common_prefixs [in] common_prefixs
	 * @param counts [in] common_prefixs的个数
	 * @retval void
	 */
	void (*set_common_prefixs)(oss_multipart_upload_listing_t *listing,
			const char **common_prefixs,
			unsigned int counts);

	/**
	 * 获得分隔符
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return 分隔符
	 * @return const char *
	 */
	const char * (*get_delimiter)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置分隔符
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param delimiter [in] 分隔符
	 * @retval void
	 */
	void (*set_delimiter)(oss_multipart_upload_listing_t *listing, const char *delimiter);

	/**
	 * 获得前缀
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @return 前缀
	 * @return const char *
	 */
	const char * (*get_prefix)(oss_multipart_upload_listing_t *listing);

	/**
	 * 设置前缀
	 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
	 * @param prefix [in] 前缀
	 * @retval void
	 */
	void (*set_prefix)(oss_multipart_upload_listing_t *listing, const char *prefix);
};

/**
 * oss_multipart_upload_listing_t构造函数
 * @return 返回一个oss_multipart_upload_listing_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_multipart_upload_listing_t *
multipart_upload_listing_initialize(void);

/**
 * oss_multipart_upload_listing_t析构函数
 * @param listing [in] 标识一个oss_multipart_upload_listing_t结构指针
 * @retval void
 * @pre listing 必须使用multipart_upload_listing_initialize的返回值
 */
extern void 
multipart_upload_listing_finalize(oss_multipart_upload_listing_t *listing);
/**@}*/
#endif
