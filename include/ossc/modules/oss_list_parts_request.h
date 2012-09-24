/*
 * =============================================================================
 *
 *       Filename:  oss_list_parts_request.h
 *
 *    Description:  list parts request implementation.
 *
 *        Created:  09/05/2012 10:41:00 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_PARTS_REQUEST_H
# error Never include <ossc/modules/oss_list_parts_request_h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_PARTS_REQUEST_H
#define OSS_LIST_PARTS_REQUEST_H
#include "ossc-config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @defgroup oss_list_parts_request_t oss_list_parts_request_t
 * @{
 */

/**
 * struct oss_list_parts_request_s 内部使用 \n
 * oss_list_parts_request_t 开发者使用
 */
typedef struct oss_list_parts_request_s oss_list_parts_request_t;

struct oss_list_parts_request_s {
	char *bucket_name;         /**< Bucket名称 */
	char *key;                 /**< Object名称 */
	unsigned int max_parts;    /**< 表示最大返回多少条记录*/
	unsigned int part_number_marker;    /**< 表示从哪个Part号码开始获取列表 */
	char *upload_id;           /** 标识Multipart上传事件的Upload ID */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_list_parts_request_t *request);

	/**
	 * 获得Object名称
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_list_parts_request_t *request);

	/**
	 * 获得max_parts
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @return max_parts
	 * @retval unsigned int
	 */
	unsigned int (*get_max_parts)(oss_list_parts_request_t *request);

	/**
	 * 获得part_number_marker
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @return part_number_marker
	 * @retval unsigned int
	 */
	unsigned int (*get_part_number_marker)(oss_list_parts_request_t *request);

	/**
	 * 获得Upload ID
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @return Upload ID
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_list_parts_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_list_parts_request_t *request,
			const char *bucket_name);

	/**
	 * 设置Object名称
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_list_parts_request_t *request,
			const char *key);

	/**
	 * 设置max_parts
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @param max_parts [in] max_parts
	 * @retval void
	 */
	void (*set_max_parts)(oss_list_parts_request_t *request,
			unsigned int max_parts);

	/**
	 * 设置part_number_marker
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @param part_number_marker [in] part_number_marker
	 * @retval void
	 */
	void (*set_part_number_marker)(oss_list_parts_request_t *request,
			unsigned int part_number_marker);

	/**
	 * 设置Upload ID
	 * @param request [in] 标识一个oss_list_parts_request_t结构指针
	 * @param upload_id [in] Upload ID
	 * @retval void
	 */
	void (*set_upload_id)(oss_list_parts_request_t *request,
			const char *upload_id);
};

/**
 * oss_list_parts_request_t构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @param upload_id [in] Upload ID
 * @return 返回一个oss_list_parts_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_list_parts_request_t *
list_parts_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id);
/**
 * oss_list_parts_request_t析构函数
 * @param request [in] 标识一个oss_list_parts_request_t结构指针
 * @retval void
 * @pre request 必须使用list_parts_request_initialize的返回值
 */
extern void
list_parts_request_finalize(
		oss_list_parts_request_t *request);

/**@}*/
#endif // OSS_LIST_PARTS_REQUEST_H 
