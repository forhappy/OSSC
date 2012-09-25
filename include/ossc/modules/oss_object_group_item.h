/*
 * =============================================================================
 *
 *       Filename:  oss_object_group_item.h
 *
 *    Description:  oss object_group_item
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_GROUP_ITEM_H
# error Never include <ossc/modules/oss_object_group_item.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_GROUP_ITEM_H 
#define OSS_OBJECT_GROUP_ITEM_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * @defgroup oss_object_group_item_t oss_object_group_item_t
 * @{
 */

/**
 * struct oss_object_group_item_s 内部使用 \n
 * oss_object_group_item_t 开发者使用
 */
typedef struct oss_object_group_item_s oss_object_group_item_t;

/**
 * post object group 需要提供的 Object 信息
 */
struct oss_object_group_item_s {
	char *etag;            /**< Object的etag */
	char *part_name;       /**< Object的name */
	int part_number;       /**< Object的number */

	/**
	 * 获得Object的etag
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @return Object的etag
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_object_group_item_t *item);

	/**
	 * 设置Object的etag
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @param etag [in] Object的etag
	 * @retval void
	 */
	void (*set_etag)(oss_object_group_item_t *item, const char *etag);

	/**
	 * 获得Object的name
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @return Object的name
	 * @retval const char *
	 */
	const char * (*get_part_name)(oss_object_group_item_t *item);

	/**
	 * 设置Object的name
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @param part_name [in] Object的name
	 * @retval void
	 */
	void (*set_part_name)(oss_object_group_item_t *item, const char *part_name);

	/**
	 * 获得Object的number
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @return Object的number
	 * @retval int
	 */
	int (*get_part_number)(oss_object_group_item_t *item);

	/**
	 * 设置Object的number
	 * @param item [in] 标识一个oss_object_group_item_t结构指针
	 * @param part_name [in] Object的number
	 * @retval void
	 */
	void (*set_part_number)(oss_object_group_item_t *item, int part_number);
};

/**
 * oss_object_group_item_t构造函数
 * @return 返回一个oss_object_group_item_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_object_group_item_t * object_group_item_initialize();

/**
 * oss_object_group_item_t析构函数
 * @param item [in] 标识一个oss_object_group_item_t结构指针
 * @retval void
 * @pre item 必须使用object_group_item_initialize的返回值
 */
extern void object_group_item_finalize(oss_object_group_item_t *item);
/**@}*/
#endif // OSS_OBJECT_GROUP_ITEM_H 

