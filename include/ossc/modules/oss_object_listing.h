/*
 * =============================================================================
 *
 *       Filename:  oss_object_listing.h
 *
 *    Description:  oss_object_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_LISTING_H
# error Never include <ossc/modules/oss_object_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_LISTING_H
#define OSS_OBJECT_LISTING_H
#define _OSS_OBJECT_SUMMARY_H
#include <ossc/modules/oss_object_summary.h>
#undef _OSS_OBJECT_SUMMARY_H
#include <ossc/ossc-config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

/**
 * @defgroup oss_object_listing_t oss_object_listing_t
 * @{
 */

/**
 * struct oss_object_listing_s 内部使用 \n
 * oss_object_listing_t 开发者使用
 */
typedef struct oss_object_listing_s oss_object_listing_t;

/**
 * 一个Bucket中所有的Object信息
 */
struct oss_object_listing_s {
	char *bucket_name;       /**< Bucket名称 */
	char *next_marker;       /**< 如果因为 max-keys 的设定无法一次完成 listing,返回结果会附加一个NextMarker */
	char *prefix;            /**< 前缀 */
	char *marker;            /**< 标识表示从哪里返回列表 */
	int max_keys;            /**< 返回 object 的最大数 */
	char * delimiter;        /**< 分隔符 */
	bool is_truncated;       /**< 标识是否有未显示的Object */

	oss_object_summary_t **summaries;      /**< 所有的Object信息 */
	unsigned int _counts_summaries;        /**< Object的数目 */

	char **common_prefixes;                 /**< 名字包含指定的前缀且第一次出现 delimiter 字符之间的 object */
	unsigned int _counts_common_prefixes;   /**< common_prefixes的数目 */

	/**
	 * 获得Bucket名称
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_object_listing_t *listing);

	/**
	 * 设置Bucket名称
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_object_listing_t *listing, const char *bucket_name);

	/**
	 * 获得next_marker
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return next_marker
	 * @retval const char *
	 */
	const char * (*get_next_marker)(oss_object_listing_t *listing);

	/**
	 * 设置next_marker
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param next_marker [in] next_marker
	 * @retval void
	 */
	void (*set_next_marker)(oss_object_listing_t *listing, const char *next_marker);

	/**
	 * 获得前缀
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return 前缀
	 * @retval const char *
	 */
	const char * (*get_prefix)(oss_object_listing_t *listing);

	/**
	 * 设置前缀
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param prefix [in] 前缀
	 * @retval void
	 */
	void (*set_prefix)(oss_object_listing_t *listing, const char *prefix);

	/**
	 * 获得marker
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return marker
	 * @retval const char *
	 */
	const char * (*get_marker)(oss_object_listing_t *listing);

	/**
	 * 设置marker
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param marker [in] marker
	 * @retval void
	 */
	void (*set_marker)(oss_object_listing_t *listing, const char *marker);

	/**
	 * 获得max_keys
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return max_keys
	 * @retval int
	 */
	int (*get_max_keys)(oss_object_listing_t *listing);

	/**
	 * 设置max_keys
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param max_keys [in] max_keys
	 * @retval void
	 */
	void (*set_max_keys)(oss_object_listing_t *listing, int max_keys);

	/**
	 * 获得分隔符
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return 分隔符
	 * @retval const char *
	 */
	const char * (*get_delimiter)(oss_object_listing_t *listing);

	/**
	 * 设置分隔符
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param delimiter [in] 分隔符
	 * @retval void
	 */
	void (*set_delimiter)(oss_object_listing_t *listing, const char *delimiter);

	/**
	 * 获得is_truncated
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @return is_truncated
	 * @retval bool
	 */
	bool (*get_is_truncated)(oss_object_listing_t *listing);

	/**
	 * 设置is_truncated
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param is_truncated [in] is_truncated
	 * @retval void
	 */
	void (*set_is_truncated)(oss_object_listing_t *listing, bool is_truncated);

	/**
	 * 获得common_prefixes
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param counts [out] common_prefixes的数目
	 * @return common_prefixes
	 * @retval const char **
	 */
	const char ** (*get_common_prefixes)(oss_object_listing_t *listing,
			unsigned int *counts);

	/**
	 * 设置common_prefixes
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param common_prefixes [in] common_prefixes
	 * @param counts [in] common_prefixes的数目
	 * @retval void
	 */
	void (*set_common_prefixes)(oss_object_listing_t *listing,
			const char **common_prefixes,
			unsigned int counts);

	/**
	 * 获得所有的Object信息
	 * @param listing [in] 标识一个oss_object_listing_t结构指针
	 * @param counts [out] 所有的Object信息的数目
	 * @return 返回一个oss_object_summary_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_object_summary_t ** (*get_summaries)(oss_object_listing_t *listing,
			unsigned int *counts);
};

/**
 * oss_object_listing_t构造函数
 * @return 返回一个oss_object_listing_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_object_listing_t *
object_listing_initialize(void);

/**
 * oss_object_listing_t析构函数
 * @param listing [in] 标识一个oss_object_listing_t结构指针
 * @retval void
 * @pre listing 必须使用object_listing_initialize的返回值
 */
extern void 
object_listing_finalize(oss_object_listing_t *listing);
/**@}*/
#endif
