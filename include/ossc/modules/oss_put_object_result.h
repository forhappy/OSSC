/*
 * =============================================================================
 *
 *       Filename:  oss_put_object_result.h
 *
 *    Description:  oss_put_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PUT_OBJECT_RESULT_H
# error Never include <ossc/modules/oss_put_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PUT_OBJECT_RESULT_H
#define OSS_PUT_OBJECT_RESULT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_put_object_result_t oss_put_object_result_t
 * @{
 */

/**
 * struct oss_put_object_result_s 内部使用 \n
 * oss_put_object_result_t 开发者使用
 */
typedef struct oss_put_object_result_s oss_put_object_result_t;

/**
 * 上传object操作的返回结果
 */
struct oss_put_object_result_s {
	char *etag; /**< 新创建的OSSObject的ETag值*/

	/**
	 * 获得新创建的OSSObject的ETag值
	 * @param result [in] 标识一个oss_put_object_result_t结构指针
	 * @return 新创建的OSSObject的ETag值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_put_object_result_t *result);

	/**
	 * 设置新创建的OSSObject的ETag值
	 * @param result [in] 标识一个oss_put_object_result_t结构指针
	 * @param etag [in] 新创建的OSSObject的ETag值
	 * @retval void
	 */
	void (*set_etag)(oss_put_object_result_t *result, const char *etag);
};

/**
 * oss_put_object_result_t 构造函数
 * @return 返回一个oss_put_object_result_t 结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_put_object_result_t *
put_object_result_initialize();

/**
 * oss_put_object_result_t 析构函数
 * @param  [in] 标识一个oss_put_object_result_t结构指针
 * @retval void
 * @pre result 必须使用put_object_result_initialize的返回值
 */
extern void 
put_object_result_finalize(oss_put_object_result_t *result);
/**@}*/
#endif
