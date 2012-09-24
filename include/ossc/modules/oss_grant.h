/*
 * =============================================================================
 *
 *       Filename:  oss_grant.h
 *
 *    Description:  oss_grant.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_GRANT_H
# error Never include <ossc/modules/oss_grant.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GRANT_H
#define OSS_GRANT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_grant_t oss_grant_t
 * @{
 */

/**
 * struct oss_grant_s 内部使用 \n
 * oss_grant_t 开发者使用
 */
typedef struct oss_grant_s oss_grant_t;

/**
 * 访问控制的授权信息
 */
struct oss_grant_s {
	char *identifier;            /**< 被授权者信息 */
	char *permission;            /**< 被授权者权限 */
	struct oss_grant_s *next;    /**< 链表指针    */

	/**
	 * 获得被授权者信息
	 * @param grant [in] 标识一个oss_grant_t结构指针
	 * @return 被授权者信息
	 * @retval const char *
	 */
	const char * (*get_identifier)(oss_grant_t *grant);

	/**
	 * 设置被授权者信息
	 * @param grant [in] 标识一个oss_grant_t结构指针
	 * @param identifier [in] 被授权者信息
	 * @retval void
	 */
	void (*set_identifier)(oss_grant_t *grant, const char * identifier);

	/**
	 * 获得被授权者权限
	 * @param grant [in] 标识一个oss_grant_t结构指针
	 * @return 被授权者权限
	 * @retval const char *
	 */
	const char * (*get_permission)(oss_grant_t *grant);

	/**
	 * 设置被授权者权限
	 * @param grant [in] 标识一个oss_grant_t结构指针
	 * @param permission [in] 被授权者权限
	 * @retval void
	 */
	void (*set_permission)(oss_grant_t *grant, const char *permission);

};

/**
 * oss_grant_t构造函数
 * @param identifier [in] 被授权者信息
 * @param permission [in] 被授权者权限
 * @return 返回一个oss_grant_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_grant_t *
grant_initialize(const char *identifier, const char *permission);

/**
 * oss_grant_t析构函数
 * @param grant [in] 标识一个oss_grant_t结构指针
 * @retval void
 * @pre grant 必须使用grant_initialize的返回值
 */
extern void 
grant_finalize(oss_grant_t *grant);
/**@}*/
#endif
