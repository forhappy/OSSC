/*
 * =============================================================================
 *
 *       Filename:  oss_part_listing.h
 *
 *    Description:  oss_part_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PART_LISTING_H
# error Never include <ossc/modules/oss_part_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PART_LISTING_H
#define OSS_PART_LISTING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  <ossc/modules/oss_owner.h>
#undef _OSS_OWNER_H

#define _OSS_PART_SUMMARY_H
#include <ossc/modules/oss_part_summary.h>
#undef _OSS_PART_SUMMARY_H

#include <ossc/ossc-config.h>

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
/**
 * @defgroup oss_part_listing_t oss_part_listing_t
 * @{
 */

/**
 * struct oss_part_listing_s 内部使用 \n
 * oss_part_listing_t 开发者使用
 */
typedef struct oss_part_listing_s oss_part_listing_t;

/**
 * 包含Multipart上传Part的表示
 */
struct oss_part_listing_s {
	char *bucket_name;                  /**< Bucket名称 */
	oss_owner_t *initiator;             /**< Object的的初始化者 */
	bool is_truncated;                  /**< 标识结果是否被截取 */
	char *key;                          /**< Object的名称 */
	int max_parts;                      /**< 请求中指定返回Part的最大个数 */
	int next_part_number_marker;        /**< 如果返回结果被截取，那么下一个Part的号码是多少 */
	oss_owner_t *owner;                 /**< Object的所有者 */
	int part_number_marker;             /**< 标识从哪里显示 */
	oss_part_summary_t **parts;         /**< PartSummary的列表 */
	int parts_number;                   /**< PartSummary的列表的数目 */
	char *storage_class;                /**< Object的存储类别 */
	char *upload_id;                    /**< 标识Multipart上传事件的Upload ID */

	/**
	 * 获得Bucket名称
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_part_listing_t *listing);

	/**
	 * 设置Bucket名称
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_part_listing_t *listing, const char *bucket_name);

	/**
	 * 获得Object的名称
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return Object的名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_part_listing_t *listing);

	/**
	 * 设置Object的名称
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param key [in] Object的名称
	 * @retval void
	 */
	void (*set_key)(oss_part_listing_t *listing, const char *key);

	/**
	 * 获得Upload ID
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return Upload ID
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_part_listing_t *listing);

	/**
	 * 设置Upload ID
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param upload_id [in] Upload ID
	 * @retval void
	 */
	void (*set_upload_id)(oss_part_listing_t *listing, const char *upload_id);

	/**
	 * 获得Object的存储类别
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return Object的存储类别
	 * @retval const char *
	 */
	const char * (*get_storage_class)(oss_part_listing_t *listing);

	/**
	 * 设置Object的存储类别
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param storage_class [in] Object的存储类别
	 * @retval void
	 */
	void (*set_storage_class)(oss_part_listing_t *listing, const char *storage_class);

	/**
	 * 获得Object的的初始化者
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return Object的的初始化者
	 * @return 返回一个 oss_owner_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_owner_t * (*get_initiator)(oss_part_listing_t *listing);

	/**
	 * 设置Object的的初始化者
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param initiator [in] Object的的初始化者
	 * @retval void
	 */
	void (*set_initiator)(oss_part_listing_t *listing, oss_owner_t *initiator);

	/**
	 * 获得请求中指定返回Part的最大个数
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return 请求中指定返回Part的最大个数
	 * @retval int
	 */
	int (*get_max_parts)(oss_part_listing_t *listing);

	/**
	 * 设置请求中指定返回Part的最大个数
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param max_parts [in] 请求中指定返回Part的最大个数
	 * @retval void
	 */
	void (*set_max_parts)(oss_part_listing_t *listing, int max_parts);

	/**
	 * 获得is_truncated
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return is_truncated
	 * @retval bool
	 */
	bool (*get_is_truncated)(oss_part_listing_t *listing);

	/**
	 * 设置is_truncated
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param is_truncated [in] is_truncated
	 * @retval void
	 */
	void (*set_is_truncated)(oss_part_listing_t *listing, bool is_truncated);

	/**
	 * 获得PartSummary的列表
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param parts_number [out] PartSummary的列表的个数
	 * @return 返回一个oss_part_summary_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_part_summary_t ** (*get_parts)(oss_part_listing_t *listing, int *parts_number);

	/**
	 * 设置PartSummary的列表
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param parts [in] PartSummary的列表
	 * @param parts_number [in] PartSummary的列表的个数
	 * @retval void
	 */
	void (*set_parts)(oss_part_listing_t *listing, oss_part_summary_t **parts, int parts_number);

	/**
	 * 获得next_part_number_marker
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return next_part_number_marker
	 * @retval int
	 */
	int (*get_next_part_number_marker)(oss_part_listing_t *listing);

	/**
	 * 设置next_part_number_marker
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param next_part_number_marker [in] next_part_number_marker
	 * @retval void
	 */
	void (*set_next_part_number_marker)(oss_part_listing_t *listing, int next_part_number_marker);

	/**
	 * 获得Object的所有者
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return 返回一个oss_owner_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_owner_t * (*get_owner)(oss_part_listing_t *listing);

	/**
	 * 设置Object的所有者
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param owner [in] bject的所有者
	 * @retval void
	 */
	void (*set_owner)(oss_part_listing_t *listing, oss_owner_t *owner);
	
	/**
	 * 获得part_number_marker
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @return part_number_marker
	 * @retval int
	 */
	int (*get_part_number_marker)(oss_part_listing_t *listing);

	/**
	 * 设置part_number_marker
	 * @param listing [in] 标识一个oss_part_listing_t结构指针
	 * @param part_number_marker [in] part_number_marker
	 * @retval void
	 */
	void (*set_part_number_marker)(oss_part_listing_t *listing, int part_number_marker);



};

/**
 * oss_part_listing_t构造函数
 * @return 返回一个oss_part_listing_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_part_listing_t *
part_listing_initialize(void);

/**
 * oss_part_listing_t析构函数
 * @param listing [in] 标识一个oss_part_listing_t结构指针
 * @retval void
 * @pre listing 必须使用part_listing_initialize的返回值
 */
extern void 
part_listing_finalize(oss_part_listing_t *listing);
/**@}*/
#endif
