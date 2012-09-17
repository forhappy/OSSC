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
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_put_object_result_t oss_put_object_result_t
 * @{
 */

/**
 * 上传object操作的返回结果
 */
typedef struct oss_put_object_result_s oss_put_object_result_t;

/**
 * 上传object操作的返回结果
 */
struct oss_put_object_result_s {
	char *etag; /**< 新创建的OSSObject的ETag值*/

	/**
	 * 返回新创建的OSSObject的ETag值
	 * @param result 指向上传object操作的返回结果结构的指针
	 * @return 返回新创建的OSSObject的ETag值
	 */
	const char * (*get_etag)(oss_put_object_result_t *result);

	/**
	 * 设置新创建的OSSObject的ETag值
	 * @param result 指向上传object操作的返回结果结构的指针
	 * @param etag 新创建的OSSObject的ETag值
	 */
	void (*set_etag)(oss_put_object_result_t *result, const char *etag);
};

/**
 * oss_put_object_result_t 构造函数
 * @return oss_put_object_result_t *指针
 * @retval 非空 表示成功
 * @retal NULL 表示失败
 */
extern oss_put_object_result_t *
put_object_result_initialize();

/**
 * oss_put_object_result_t 析构函数
 * @param result oss_put_object_result_t *指针
 * 该指针需由put_object_result_initialize返回
 */
extern void 
put_object_result_finalize(oss_put_object_result_t *result);
/**@}*/
#endif
