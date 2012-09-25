/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_result.h
 *
 *    Description:  oss_copy_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COPY_OBJECT_RESULT_H
# error Never include <modules/oss_copy_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COPY_OBJECT_RESULT_H
#define OSS_COPY_OBJECT_RESULT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_copy_object_result_t oss_copy_object_result_t
 * @{
 */

/**
 * struct oss_copy_object_result_s 内部使用 \n
 * oss_copy_object_result_t 开发者使用
 */
typedef struct oss_copy_object_result_s oss_copy_object_result_t;

/**
 * 拷贝一个在OSS上已经存在的Object成另外一个Object的请求结果
 */
struct oss_copy_object_result_s {
	char *etag;       /**< 新Object的ETag值 */
	char *last_modified;   /**< 新Object的最后修改时间 */

	/**
	 * 获得新Object的ETag值
	 * @param result [in] 标识一个oss_copy_object_result_t的结构指针
	 * @return 新Object的ETag值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_copy_object_result_t *result);

	/**
	 * 设置新Object的ETag值
	 * @param result [in] 标识一个oss_copy_object_result_t的结构指针
	 * @param etag [in] 新Object的ETag值
	 * @retval void
	 */
	void (*set_etag)(oss_copy_object_result_t *result, const char * etag);

	/**
	 * 获得新Object的最后修改时间
	 * @param result [in] 标识一个oss_copy_object_result_t的结构指针
	 * @return 新Object的最后修改时间
	 * @retval const char *
	 */
	const char * (*get_last_modified)(oss_copy_object_result_t *result);

	/**
	 * 设置新Object的最后修改时间
	 * @param result [in] 标识一个oss_copy_object_result_t的结构指针
	 * @param last_modified [in] 新Object的最后修改时间
	 * @retval void
	 */
	void (*set_last_modified)(oss_copy_object_result_t *result, const char *last_modified);

};

/**
 * oss_copy_object_result_t 构造函数
 * @return 返回一个oss_copy_object_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_copy_object_result_t *
copy_object_result_initialize(void);

/**
 * oss_copy_object_result_t 析构函数
 * @param result [in] 标识一个oss_copy_object_result_t的结构指针
 * @retval void
 * @pre result 必须使用copy_object_result_initialize的返回值
 */
extern void 
copy_object_result_finalize(oss_copy_object_result_t *result);
/**@}*/
#endif
