/*
 * =============================================================================
 *
 *       Filename:  oss_owner.h
 *
 *    Description:  oss owner.
 *
 *        Created:  09/05/2012 09:55:06 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_OWNER_H
# error Never include <ossc/modules/oss_owner.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OWNER_H
#define OSS_OWNER_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_owner_t oss_owner_t
 * @{
 */

/**
 * struct oss_owner_s 内部使用 \n
 * oss_owner_t 开发者使用
 */
typedef struct oss_owner_s oss_owner_t;

/**
 * 表示OSS Bucket的所有者
 */
struct oss_owner_s {
	char *id;                /**< 所有者的ID */
	char *display_name;      /**< 所有者的显示名称 */

	/**
	 * 获得所有者的ID
	 * @param owner [in] 标识一个oss_owner_t结构指针
	 * @return 所有者的ID
	 * @retval const char *
	 */
	const char * (*get_id)(oss_owner_t *owner);

	/**
	 * 获得所有者的显示名称
	 * @param owner [in] 标识一个oss_owner_t结构指针
	 * @return 所有者的显示名称
	 * @retval const char *
	 */
	const char * (*get_display_name)(oss_owner_t *owner);

	/**
	 * 设置所有者的ID
	 * @param owner [in] 标识一个oss_owner_t结构指针
	 * @param id [in] 所有者的ID
	 * @retval void
	 */
	void (*set_id)(oss_owner_t *owner, const char *id);

	/**
	 * 设置所有者的显示名称
	 * @param owner [in] 标识一个oss_owner_t结构指针
	 * @param name [in] 所有者的显示名称
	 * @retval void
	 */
	void (*set_display_name)(oss_owner_t *owner, const char *name);
};

/**
 * oss_owner_t构造函数
 * @return 返回一个oss_owner_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_owner_t *
owner_initialize(void);

/**
 * oss_owner_t带ID 和display name的构造函数
 * @param id [in] 所有者的ID
 * @param name [in] 所有者的显示名称
 * @return 返回一个oss_owner_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_owner_t *
owner_initialize_with_id(const char *id,
		const char *name);

/**
 * oss_owner_t析构函数
 * @param owner [in] 标识一个oss_owner_t结构指针
 * @retval void
 * @pre owner 必须使用owner_initialize的返回值
 */
extern void
owner_finalize(oss_owner_t *owner);
/**@}*/
#endif
