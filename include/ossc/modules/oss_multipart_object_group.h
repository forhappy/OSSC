/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_object_group.h
 *
 *    Description:  oss multipart_object_group
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_MULTIPART_OBJECT_GROUP_H
# error Never include <ossc/modules/oss_multipart_object_group.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_OBJECT_GROUP_H
#define OSS_MULTIPART_OBJECT_GROUP_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * @defgroup oss_multipart_object_group_t oss_multipart_object_group_t
 * @{
 */

/**
 * struct oss_multipart_object_group_s 内部使用 \n
 * oss_multipart_object_group_t 开发者使用
 */
typedef struct oss_multipart_object_group_s oss_multipart_object_group_t;

/**
 * object group中每个object的信息
 */
struct oss_multipart_object_group_s {
	char *etag;              /**< Object的etag */
	char *part_name;         /**< Object的name */
	int part_number;         /**< Object的number */
	unsigned int part_size;        /**< Object的size */

	/**
	 * 获得Object的etag
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @return Object的etag
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_multipart_object_group_t *group);

	/**
	 * 设置Object的etag
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @param etag [in] Object的etag
	 * @retval void
	 */
	void (*set_etag)(oss_multipart_object_group_t *group, const char *etag);

	/**
	 * 获得Object的name
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @return Object的name
	 * @retval const char *
	 */
	const char * (*get_part_name)(oss_multipart_object_group_t *group);

	/**
	 * 设置Object的name
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @param part_name [in] Object的name
	 * @retval void
	 */
	void (*set_part_name)(oss_multipart_object_group_t *group, const char *part_name);

	/**
	 * 获得Object的number
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @return Object的number
	 * @retval int
	 */
	int (*get_part_number)(oss_multipart_object_group_t *group);

	/**
	 * 设置Object的number
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @param part_number [in] Object的number
	 * @retval void
	 */
	void (*set_part_number)(oss_multipart_object_group_t *group, int part_number);

	/**
	 * 获得Object的size
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @return Object的size
	 * @retval unsigned int
	 */
	unsigned int (*get_part_size)(oss_multipart_object_group_t *group);

	/**
	 * 设置Object的size
	 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
	 * @param part_size [in] Object的size
	 * @retval void
	 */
	void (*set_part_size)(oss_multipart_object_group_t *group, unsigned int part_size);

};

/**
 * oss_multipart_object_group_t构造函数
 * @return 返回一个oss_multipart_object_group_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_multipart_object_group_t * multipart_object_group_initialize();

/**
 * oss_multipart_object_group_t析构函数
 * @param group [in] 标识一个oss_multipart_object_group_t结构指针
 * @retval void
 * @pre group 必须使用multipart_object_group_initialize的返回值
 */
extern void multipart_object_group_finalize(oss_multipart_object_group_t *group);
/**@}*/
#endif // OSS_MULTIPART_OBJECT_GROUP_H

