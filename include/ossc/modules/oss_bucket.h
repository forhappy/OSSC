/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.h
 *
 *    Description:  oss_bucket.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_BUCKET_H
# error Never include <ossc/modules/oss_bucket.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_BUCKET_H
#define OSS_BUCKET_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  "ossc/modules/oss_owner.h"
#undef _OSS_OWNER_H

/**
 * @addtogroup oss_bucket_t oss_bucket_t
 * @{
 * Bucket是OSS上的命名空间, Bucket名在整个 OSS 中具有全局唯一性，且不能修改；
 * 存储在OSS上的每个Object必须都包含在某个Bucket中。
 * 一个应用，例如图片分享网站，可以对应一个或多个 Bucket。
 * 一个用户最多可创建 10 个Bucket， 但每个Bucket 中存放的Object的数量和大小总和没有限制，
 * 用户不需要考虑数据的可扩展性。
 *
 * Bucket 命名规范
 * - 只能包括小写字母，数字，下划线（_）和短横线（-）
 * - 必须以小写字母或者数字开头
 * - 长度必须在 3-255 字节之间
 */

/**
 * struct oss_bucket_s 内部使用 \n
 * oss_bucket_t 开发者使用
 */
typedef struct oss_bucket_s oss_bucket_t;

struct oss_bucket_s {
	char *create_date;
	char *name;
	oss_owner_t *owner;

	const char * (*get_create_date)(oss_bucket_t *bucket);
	void (*set_create_date)(oss_bucket_t *bucket, const char *create_date);

	const char * (*get_name)(oss_bucket_t *bucket);
	void (*set_name)(oss_bucket_t *bucket, const char * name);

	oss_owner_t * (*get_owner)(oss_bucket_t *bucket);
	void (*set_owner)(oss_bucket_t *bucket, oss_owner_t *owner);
};

extern oss_bucket_t *
bucket_initialize(void);

extern oss_bucket_t *
bucket_initialize_with_name(const char *name);

extern void 
bucket_finalize(oss_bucket_t *bucket);

/**@}*/

#endif
